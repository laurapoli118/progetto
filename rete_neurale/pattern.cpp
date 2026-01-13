#include "pattern.hpp"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <fstream>
#include <random>
#include <algorithm>
#include <fstream>
#include <string>
#include <cassert>

// RICORDARSI DI AGGIUNGERE ASSERT E EXCEPTION PER GESTIRE RUNTIME

Pattern::Pattern(int size) : size_(size), numNeurons_(size * size) {
    assert(size > 0 && "Error: size must be greater than 0.");
    neurons_.resize(numNeurons_, 0); // costruttore che inizializza il vettore con zeri, vedi giù per diff. resize e reserve
}

void Pattern::setNeuron(unsigned index, int value) { // FUNZIONE SETTER 
    assert(index <= numNeurons_ && "Error: can't set an inexisting nueron.");
    neurons_[index] = value;
}

int Pattern::getNeuron(unsigned index) const { return neurons_[index]; } // FUNZIONI GETTERS

unsigned Pattern::getSize() const { return size_; }

unsigned Pattern::getNumNeurons() const { return numNeurons; }

const std::vector<int> &Pattern::getData() const { return neurons; }

void Pattern::addNoise(float noisePerc) { // POI LOLLO QUANDO VUOI MI SPIEGHI STA MERDA E VALUTIAMO QUALE FARE
    std::transform(neurons.begin(), neurons.end(), neurons.begin(), [=](int currentNeuron) { // std::transform prende: Inizio, Fine, Dove Scrivere, La Funzione da applicare
        float random = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
        if (random < noisePerc) {
                return -currentNeuron;
            } else {
                return currentNeuron;
            } 
        }
    );
}

sf::Image Pattern::resize(const sf::Image& original) const {
    if (original.getSize().x == size_ && original.getSize().y == size_) {
        return original;
    } //controllo se va già bene

    sf::Image risized; // creo una tela delle dimensioni che vogliamo
    risized.create(size_, size_, sf::Color::Black);

    sf::Vector2u orgSize = original.getSize();
    
    for (unsigned y = 0; y < size_; ++y) {
        for (unsigned x = 0; x < size_; ++x) {
            unsigned orgX = x * orgSize.x / size_;
            unsigned orgY = y * orgSize.y / size_;
            risized.setPixel(x, y, original.getPixel(orgX, orgY));
        }
    }
    return risized;
}

void Pattern::display() const {
    sf::Image visibleImage;
    visibleImage.create(size_, size_); // trasforma il vettore in un'immagine

    for (unsigned int i = 0; i < numNeurons_; i++) {
        unsigned int x = i % size_;
        unsigned int y = i / size_;
        if (neurons_[i] == 1) {
            visibleImage.setPixel(x, y, sf::Color::Black); // li giro di nuovo così vengono i colori sensati
        } else {
            visibleImage.setPixel(x, y, sf::Color::White);
        }
    } // colora

    sf::Texture texture;
    texture.loadFromImage(visibleImage);
    sf::Sprite sprite(texture);
    sprite.setScale(10.0f, 10.0f); // ingrandisce che sarebbe 50 pixel se no

    sf::RenderWindow window(sf::VideoMode(size_ * 10, size_ * 10), "Pattern Preview");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) { // praticamente ogni tipo secondo controlla se fai qualcosa sul pc e se premi X allora esce
            if (event.type == sf::Event::Closed) // che merda c++
                window.close();
        }
        window.clear();     
        window.draw(sprite); 
        window.display();    
    }
}

bool Pattern::loadFromImage(const std::string& imgName) {
    sf::Image startingImg;
    if (!startingImg.loadFromFile("./images/" + imgName)) {
        std::cerr << "Error: Impossible to find the file " << imgName << std::endl;
        return false;
    } // autoesplicativo

    sf::Image image = Pattern::resize(startingImg);

    std::vector<int> pixelLuminance; // vettore temporaneo per salvare le luminosità dei pixel
    pixelLuminance.reserve(numNeurons_); // sicuro lo chiede, questo 'riserva' tot spazi ma vuoti, mentre .resize riempe tot spazi di 0.
    
    long luminositySum = 0;

    for (unsigned int y = 0; y < size_; y++) {
        for (unsigned int x = 0; x < size_; x++) {
            sf::Color c = image.getPixel(x, y);
            int lum = 0.299 * c.r + 0.587 * c.g + 0.114 * c.b; // sono le proporzioni di importanza per l'occhio del rosso, giallo e blu
            pixelLuminance.push_back(lum);
            luminositySum += lum;
        }
    }

    int turningPoint = 0.8 * luminositySum / numNeurons_;
    for (size_t i = 0; i < pixelLuminance.size(); ++i) {
        if (pixelLuminance[i] > turningPoint) {
            neurons_[i] = -1; // per le reti è meglio (non chiedetemi perchè) avere 1 nero, tipo riconosce i bordi come attivo e sfondo spento
        } else {
            neurons_[i] = 1;
        }
    }

    return true;
}