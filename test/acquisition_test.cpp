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
  unsigned bigSize = 10;
  sf::Image bigImage;
  bigImage.create(bigSize, bigSize, sf::Color::Black);

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

  unsigned smallSize = 2;
  sf::Image resized  = hp::resize(bigImage, smallSize);

  SUBCASE("Le dimensioni sono ridotte correttamente")
  {
    CHECK(resized.getSize().x == smallSize);
    CHECK(resized.getSize().y == smallSize);
  }

  SUBCASE("I colori dominanti sono preservati")
  {
    CHECK(resized.getPixel(0, 0) == sf::Color::Red);

    CHECK(resized.getPixel(1, 0) == sf::Color::Blue);

    CHECK(resized.getPixel(0, 1) == sf::Color::Green);

    CHECK(resized.getPixel(1, 1) == sf::Color::Yellow);
  }
}

TEST_CASE("resize, compressione immagine rettangolare")
{
  unsigned orgW = 20;
  unsigned orgH = 10;
  sf::Image rectImg;
  rectImg.create(orgW, orgH, sf::Color::White);

  // 2. Inseriamo un pixel rosso
  // Calcoliamo la posizione in modo che finisca esattamente in (2, 2) nel
  // target 5x5. X: 2 * (20 / 5) = 8 Y: 2 * (10 / 5) = 4
  rectImg.setPixel(8, 4, sf::Color::Red);

  // Ridimensioniamo a 5x5
  unsigned targetSize = 5;
  sf::Image resized   = hp::resize(rectImg, targetSize);

  // Verifiche
  SUBCASE("La dimensione finale deve essere quadrata 5x5")
  {
    CHECK(resized.getSize().x == targetSize);
    CHECK(resized.getSize().y == targetSize);
  }

  SUBCASE("Il pixel rosso deve essere sopravvissuto nella posizione prevista")
  {
    CHECK(resized.getPixel(2, 2) == sf::Color::Red);
  }

  SUBCASE("Il resto dell'immagine deve essere rimasto bianco")
  {
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

  hp::Pattern p(2);

  SUBCASE("Distinzione Bianco/Nero (Binarizzazione)")
  {
    std::string filename = "test_bw";
    sf::Image img;
    img.create(2, 2, sf::Color::White);
    img.setPixel(0, 0, sf::Color::Black);

    img.saveToFile(dirPath + "/" + filename + ".png");

    bool success = hp::loadFromImage(filename, p);
    CHECK(success == true);

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

    hp::loadFromImage(filename, p);

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
    bool result = hp::loadFromImage(filename, p);
    CHECK(result == false);

    fs::remove(fullPath);
  }
}