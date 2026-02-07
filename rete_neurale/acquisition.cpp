#include "acquisition.hpp"
#include "pattern.hpp"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cassert>
#include <cmath>

#ifndef ROOT_DIR
#  define ROOT_DIR "."
#endif

namespace hp {

sf::Image resize(const sf::Image& original, unsigned size)
{
  {
    if (original.getSize().x == size && original.getSize().y == size) {
      return original;
    }

    sf::Image resized; // creo una tela delle dimensioni che vogliamo
    resized.create(size, size, sf::Color::Black);

    sf::Vector2u orgSize =
        original.getSize(); // Vector2u è una struct con due coordinate x e y
                            // che devono essere unsigned int

    for (unsigned y = 0; y < size; ++y) {
      for (unsigned x = 0; x < size; ++x) {
        unsigned orgX = x * orgSize.x / size;
        unsigned orgY = y * orgSize.y / size;
        resized.setPixel(x, y, original.getPixel(orgX, orgY));
      }
    }
    return resized;
  }
}

void display(const Pattern& pattern)
{
  unsigned size       = pattern.getSize();
  unsigned numNeurons = pattern.getNumNeurons();
  sf::Image visibleImage;
  visibleImage.create(size, size); // trasforma il vettore in un'immagine

  for (unsigned int i = 0; i < numNeurons; i++) {
    unsigned int x = i % size; // calcola la coordinata x e y del pixel
                               // corrispondente al neurone i
    unsigned int y = i / size;
    if (pattern.getNeuron(i) == 1) {
      visibleImage.setPixel(x, y, sf::Color::Black);
    } else {
      visibleImage.setPixel(x, y, sf::Color::White);
    }
  } // colora

  sf::Texture texture;
  texture.loadFromImage(visibleImage);
  sf::Sprite sprite(texture);
  sprite.setScale(
      640.0f / static_cast<float>(size),
      640.0f
          / static_cast<float>(size)); // ingrandisce che sarebbe 50 pixel se no

  sf::RenderWindow window(sf::VideoMode(640.0f, 640.0f), "Pattern Preview");

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
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
  }

  sf::Image image = resize(startingImg, size);

  std::vector<int>
      pixelLuminance; // vettore temporaneo per salvare le luminosità dei pixel
  pixelLuminance.reserve(numNeurons);
  long luminositySum = 0;

  for (unsigned int y = 0; y < size; y++) {
    for (unsigned int x = 0; x < size; x++) {
      sf::Color c = image.getPixel(x, y);
      int lum     = static_cast<int>(
          0.299 * c.r + 0.587 * c.g
          + 0.114 * c.b); // sono le proporzioni di importanza per l'occhio del
                              // rosso, giallo e blu
      pixelLuminance.push_back(lum);
      luminositySum += lum;
    }
  }
  int turningPoint = static_cast<int>(0.8 * static_cast<double>(luminositySum)
                                      / static_cast<double>(numNeurons));
  for (size_t i = 0; i < pixelLuminance.size(); ++i) {
    if (pixelLuminance[i] > turningPoint) {
      pattern.setNeuron(static_cast<unsigned>(i), -1);
    } else {
      pattern.setNeuron(static_cast<unsigned>(i), 1);
    }
  }
  return true;
}

} // namespace hp