#include "acquisition.hpp"
#include "pattern.hpp"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cassert>
#include <cmath>

#ifndef ROOT_DIR
#define ROOT_DIR "."
#endif

namespace Acquisition {

sf::Image resize(const sf::Image& original, unsigned size)
{
  {
    if (original.getSize().x == size && original.getSize().y == size) {
      return original;
    } // controllo se va già bene

    sf::Image risized; // creo una tela delle dimensioni che vogliamo
    risized.create(size, size, sf::Color::Black);

    sf::Vector2u orgSize = original.getSize();

    for (unsigned y = 0; y < size; ++y) {
      for (unsigned x = 0; x < size; ++x) {
        unsigned orgX = x * orgSize.x / size;
        unsigned orgY = y * orgSize.y / size;
        risized.setPixel(x, y, original.getPixel(orgX, orgY));
      }
    }
    return risized;
  }
}

void display(unsigned size, const std::vector<int>& data)
{
  unsigned numNeurons = size * size;
  sf::Image visibleImage;
  visibleImage.create(size, size); // trasforma il vettore in un'immagine

  for (unsigned int i = 0; i < numNeurons; i++) {
    unsigned int x = i % size;
    unsigned int y = i / size;
    if (data[i] == 1) {
      visibleImage.setPixel(
          x, y,
          sf::Color::Black); // li giro di nuovo così vengono i colori sensati
    } else {
      visibleImage.setPixel(x, y, sf::Color::White);
    }
  } // colora

  sf::Texture texture;
  texture.loadFromImage(visibleImage);
  sf::Sprite sprite(texture);
  sprite.setScale(640.0f / size,
                  640.0f / size); // ingrandisce che sarebbe 50 pixel se no

  sf::RenderWindow window(sf::VideoMode(640.0f, 640.0f), "Pattern Preview");

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(
        event)) { // praticamente ogni tipo secondo controlla se fai qualcosa
                  // sul pc e se premi X allora esce
      if (event.type == sf::Event::Closed) // che merda c++
        window.close();
    }
    window.clear();
    window.draw(sprite);
    window.display();
  }
}

bool loadFromImage(const std::string& filename, Pattern& pattern)
{
    unsigned size = pattern.getSize();
  sf::Image startingImg;
  unsigned numNeurons = pattern.getNumNeurons();
  
  if (!startingImg.loadFromFile(std::string(ROOT_DIR) + "/images/" + filename
                                + ".png")) {
    std::cerr << "Error: Impossible to find the file " << filename << std::endl;
    return false;
  } // autoesplicativo

  sf::Image image = resize(startingImg, size);

  std::vector<int>
      pixelLuminance; // vettore temporaneo per salvare le luminosità dei pixel
  pixelLuminance.reserve(
      numNeurons); // sicuro lo chiede, questo 'riserva' tot spazi ma vuoti,
                   // mentre .resize riempe tot spazi di 0.

  long luminositySum = 0;

  for (unsigned int y = 0; y < size; y++) {
    for (unsigned int x = 0; x < size; x++) {
      sf::Color c = image.getPixel(x, y);
      int lum     = 0.299 * c.r + 0.587 * c.g
              + 0.114 * c.b; // sono le proporzioni di importanza per l'occhio
                             // del rosso, giallo e blu
      pixelLuminance.push_back(lum);
      luminositySum += lum;
    }
  }

  int turningPoint = 0.8 * luminositySum / numNeurons;
  for (size_t i = 0; i < pixelLuminance.size(); ++i) {
    if (pixelLuminance[i] > turningPoint) {
      pattern.setNeuron(i, -1); // per le reti è meglio (non chiedetemi perchè) avere 1 nero, tipo
           // per le reti è meglio (non chiedetemi perchè) avere 1 nero, tipo
              // riconosce i bordi come attivo e sfondo spento
    } else {
      pattern.setNeuron(i, 1);
    }
  }

  return true;
}

} 