#include "acquisition.hpp"
#include "doctest.h"
#include "pattern.hpp"
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <fstream>

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

TEST_CASE("resize, compressione immagine rettangolare") {
    // 1. Setup: Creiamo immagine rettangolare 20x10
    unsigned orgW = 20;
    unsigned orgH = 10;
    sf::Image rectImg;
    rectImg.create(orgW, orgH, sf::Color::White); // Sfondo tutto bianco

    // 2. Inseriamo un pixel rosso
    // Calcoliamo la posizione in modo che finisca esattamente in (2, 2) nel target 5x5.
    // X: 2 * (20 / 5) = 8
    // Y: 2 * (10 / 5) = 4
    rectImg.setPixel(8, 4, sf::Color::Red);

    // 3. Azione: Ridimensioniamo a 5x5
    unsigned targetSize = 5;
    sf::Image resized = Acquisition::resize(rectImg, targetSize);

    // 4. Verifiche
    SUBCASE("La dimensione finale deve essere quadrata 5x5") {
        CHECK(resized.getSize().x == targetSize);
        CHECK(resized.getSize().y == targetSize);
    }

    SUBCASE("Il pixel rosso deve essere sopravvissuto nella posizione prevista") {
        // Verifichiamo che il pixel in (2, 2) sia diventato Rosso
        CHECK(resized.getPixel(2, 2) == sf::Color::Red);
    }

    SUBCASE("Il resto dell'immagine deve essere rimasto bianco") {
        // Controllo paranoico: verifichiamo che un pixel vicino (es. 1,1) sia ancora bianco
        // Questo conferma che le coordinate X e Y non si sono mescolate
        CHECK(resized.getPixel(1, 1) == sf::Color::White);
    }
}
/*
TEST_CASE("Acquisition::loadFromImage - Caricamento e Binarizzazione")
{
  // Definiamo il percorso che il tuo codice si aspetta.
  // Il tuo codice cerca in: ROOT_DIR + "/images/" + filename + ".png"
  // Nota: ROOT_DIR nei test è definita da CMake come la cartella del progetto.

  std::string testDir = "images";
  std::string testFile =
      "test_dummy"; // Senza estensione, la aggiunge la tua funzione
  std::string fullPath = testDir + "/" + testFile + ".png";

  // Setup: Creiamo la cartella immagini se non esiste
  if (!fs::exists(testDir)) {
    fs::create_directory(testDir);
  }

  // Creiamo un'immagine fittizia nera e bianca
  sf::Image img;
  img.create(2, 2, sf::Color::White);   // Sfondo bianco
  img.setPixel(0, 0, sf::Color::Black); // Un pixel nero
  img.saveToFile(fullPath);

  // Prepariamo il pattern che riceverà i dati
  Pattern p(2); // Dimensione 2 (4 neuroni totali)

  SUBCASE("Caricamento file esistente")
  {
    // La funzione deve restituire true
    bool result = Acquisition::loadFromImage(testFile, p);
    CHECK(result == true);

    // Verifica che i dati siano stati caricati nel pattern
    // Il pixel nero (0,0) dovrebbe diventare -1 (o 1 a seconda della tua logica
    // inversa) Nel tuo codice: if (pixel > turningPoint) -> -1. Nero ha
    // luminanza bassa, Bianco alta. ATTENZIONE: La tua logica dice "se
    // luminanza > turningPoint (chiaro) metti -1". Controlliamo cosa succede
    // effettivamente:

    // Questo check dipende dalla tua formula di binarizzazione in
    // acquisition.cpp Se non sei sicuro, controlla che non siano tutti uguali
    bool tuttiUguali = true;
    int primoValore  = p.getNeuron(0);
    for (unsigned i = 1; i < p.getNumNeurons(); i++) {
      if (p.getNeuron(i) != primoValore)
        tuttiUguali = false;
    }
    CHECK(tuttiUguali == false); // Deve aver distinto il nero dal bianco
  }

  SUBCASE("Caricamento file inesistente")
  {
    Pattern p_empty(2);
    // Proviamo a caricare un file che non c'è
    bool result = Acquisition::loadFromImage("file_che_non_esiste", p_empty);
    CHECK(result
          == false); // Deve restituire false e stampare errore su terminale
  }

  // Teardown: Pulizia (Rimuoviamo il file creato per non sporcare il progetto)
  fs::remove(fullPath);
  // Rimuoviamo la cartella solo se è vuota (per non cancellare le tue immagini
  // vere!)
  if (fs::is_empty(testDir)) {
    fs::remove(testDir);
  }
}*/