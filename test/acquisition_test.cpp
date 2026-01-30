#include "acquisition.hpp"
#include "doctest.h"
#include "pattern.hpp"
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <fstream>

#ifndef ROOT_DIR
#  define ROOT_DIR "."
#endif

// Namespace alias per comodità
namespace fs = std::filesystem;

TEST_CASE("Resize, Compressione Immagine caso base)")
{
  // 1. Creiamo un'immagine GRANDE (10x10)
  unsigned bigSize = 10;
  sf::Image bigImage;
  bigImage.create(bigSize, bigSize, sf::Color::Black);

  // 2. Coloriamo l'immagine a "blocchi" (4 quadranti)
  // Quadrante in alto a sinistra: ROSSO
  for (unsigned x = 0; x < bigSize / 2; x++) {
    for (unsigned y = 0; y < bigSize / 2; y++) {
      bigImage.setPixel(x, y, sf::Color::Red);
    }
  }
  // Quadrante in alto a destra: BLU
  for (unsigned x = bigSize / 2; x < bigSize; x++) {
    for (unsigned y = 0; y < bigSize / 2; y++) {
      bigImage.setPixel(x, y, sf::Color::Blue);
    }
  }
  // Quadrante in basso a sinistra: VERDE
  for (unsigned x = 0; x < bigSize / 2; x++) {
    for (unsigned y = bigSize / 2; y < bigSize; y++) {
      bigImage.setPixel(x, y, sf::Color::Green);
    }
  }
  // Quadrante in basso a destra: GIALLO
  for (unsigned x = bigSize / 2; x < bigSize; x++) {
    for (unsigned y = bigSize / 2; y < bigSize; y++) {
      bigImage.setPixel(x, y, sf::Color::Yellow);
    }
  }

  // 3. Eseguiamo il DOWNSCALING a (2x2)
  unsigned smallSize = 2;
  sf::Image resized  = Acquisition::resize(bigImage, smallSize);

  SUBCASE("Le dimensioni sono ridotte correttamente")
  {
    CHECK(resized.getSize().x == smallSize);
    CHECK(resized.getSize().y == smallSize);
  }

  SUBCASE("I colori dominanti sono preservati")
  {
    // Alto-Sinistra deve essere Rosso
    CHECK(resized.getPixel(0, 0) == sf::Color::Red);

    // Alto-Destra deve essere Blu
    CHECK(resized.getPixel(1, 0) == sf::Color::Blue);

    // Basso-Sinistra deve essere Verde
    CHECK(resized.getPixel(0, 1) == sf::Color::Green);

    // Basso-Destra deve essere Giallo
    CHECK(resized.getPixel(1, 1) == sf::Color::Yellow);
  }
}

TEST_CASE("resize, compressione immagine rettangolare")
{
  // 1. Setup: Creiamo immagine rettangolare 20x10
  unsigned orgW = 20;
  unsigned orgH = 10;
  sf::Image rectImg;
  rectImg.create(orgW, orgH, sf::Color::White); // Sfondo tutto bianco

  // 2. Inseriamo un pixel rosso
  // Calcoliamo la posizione in modo che finisca esattamente in (2, 2) nel
  // target 5x5. X: 2 * (20 / 5) = 8 Y: 2 * (10 / 5) = 4
  rectImg.setPixel(8, 4, sf::Color::Red);

  // 3. Azione: Ridimensioniamo a 5x5
  unsigned targetSize = 5;
  sf::Image resized   = Acquisition::resize(rectImg, targetSize);

  // 4. Verifiche
  SUBCASE("La dimensione finale deve essere quadrata 5x5")
  {
    CHECK(resized.getSize().x == targetSize);
    CHECK(resized.getSize().y == targetSize);
  }

  SUBCASE("Il pixel rosso deve essere sopravvissuto nella posizione prevista")
  {
    // Verifichiamo che il pixel in (2, 2) sia diventato Rosso
    CHECK(resized.getPixel(2, 2) == sf::Color::Red);
  }

  SUBCASE("Il resto dell'immagine deve essere rimasto bianco")
  {
    // Controllo paranoico: verifichiamo che un pixel vicino (es. 1,1) sia
    // ancora bianco Questo conferma che le coordinate X e Y non si sono
    // mescolate
    CHECK(resized.getPixel(1, 1) == sf::Color::White);
  }
}
TEST_CASE("loadFromImage logica")
{
  // Setup ambiente: assicura che la cartella images esista
  std::string dirPath = std::string(ROOT_DIR) + "/images";
  if (!fs::exists(dirPath)) {
    fs::create_directories(dirPath);
  }

  Pattern p(2); // Pattern 2x2 (4 neuroni)

  SUBCASE("Distinzione Bianco/Nero (Binarizzazione)")
  {
    std::string filename = "test_bw";
    sf::Image img;
    img.create(2, 2, sf::Color::White);   // Sfondo bianco
    img.setPixel(0, 0, sf::Color::Black); // Un pixel nero in (0,0)

    img.saveToFile(dirPath + "/" + filename + ".png");

    bool success = Acquisition::loadFromImage(filename, p);
    CHECK(success == true);

    // Verifica logica:
    // Nero -> Luminosità bassa -> Sotto soglia -> 1
    // Bianco -> Luminosità alta -> Sopra soglia -> -1
    CHECK(p.getNeuron(0) == 1);
    CHECK(p.getNeuron(1) == -1);
    CHECK(p.getNeuron(2) == -1);
    CHECK(p.getNeuron(3) == -1);

    fs::remove(dirPath + "/" + filename + ".png");
  }

  SUBCASE("Zero Contrasto (immagine grigia uniforme)")
  {
    std::string filename = "test_gray";
    sf::Image img;
    // Grigio medio uniforme (RGB 100,100,100)
    img.create(2, 2, sf::Color(100, 100, 100));

    img.saveToFile(dirPath + "/" + filename + ".png");

    Acquisition::loadFromImage(filename, p);

    // Verifica logica:
    // Media = 100. Soglia = 0.8 * 100 = 80.
    // Valore pixel (100) > Soglia (80) -> Considerato Sfondo (-1)
    for (unsigned i = 0; i < p.getNumNeurons(); ++i) {
      CHECK(p.getNeuron(i) == -1);
    }

    fs::remove(dirPath + "/" + filename + ".png");
  }

  SUBCASE("Gestione File Corrotto (Fake PNG)")
  {
    std::string filename = "test_fake";
    std::string fullPath = dirPath + "/" + filename + ".png";

    // Creiamo un file di testo mascherato da PNG
    std::ofstream outfile(fullPath);
    outfile << "Questo non è un'immagine valida.";
    outfile.close();

    // Deve fallire elegantemente restituendo false, senza crashare
    bool result = Acquisition::loadFromImage(filename, p);
    CHECK(result == false);

    fs::remove(fullPath);
  }
}