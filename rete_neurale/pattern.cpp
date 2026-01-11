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

// RICORDARSI DI METTERE TUTTE LE ASSERT E EXCEPTION PER GESTIRE RUNTIME

Pattern::Pattern(int size) : lato(size), numNeurons(size * size)
{
    assert(size > 0 && "Error: size must be greater than 0.");
    neurons.resize(numNeurons, 0); // Costruttore che inizializza il vettore con zeri
}

// FUNZIONI PER SETTARE

void Pattern::setNeuron(unsigned index, int value)
{
    assert(index <= numNeurons && "Error: can't set an inexisting nueron.");
    neurons[index] = value;
}

// FUNZIONI GETTERS

int Pattern::getNeuron(unsigned index) const { return neurons[index]; }

unsigned Pattern::getLato() const { return lato; }

unsigned Pattern::getNumNeurons() const { return numNeurons; }

const std::vector<int> &Pattern::getData() const { return neurons; }

// FUNZIONE NOISE

void Pattern::addNoise(float noisePerc) { // QUA HO USATO UNA LAMBDA E UN ALGORITMO, VOLENDO SI PUO' FARE ANCHE CON UN CICLO FOR
    // std::transform prende: Inizio, Fine, Dove Scrivere, La Funzione da applicare
    std::transform(neurons.begin(), neurons.end(), neurons.begin(), [=](int currentNeuron) {
        
        float random = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
        if (random < noisePerc)
            {
                return -currentNeuron;
            }
            else
            {
                return currentNeuron;
            } 
        }
    );
}

sf::Image Pattern::resize(const sf::Image& originale) const {
    if (originale.getSize().x == lato && originale.getSize().y == lato) {
        return originale;
    } //controllo se va già bene

    sf::Image ridimensionata; // creo una tela delle dimensioni che vogliamo
    ridimensionata.create(lato, lato, sf::Color::Black);

    sf::Vector2u orgSize = originale.getSize();
    
    for (unsigned y = 0; y < lato; ++y) {
        for (unsigned x = 0; x < lato; ++x) {
            // Mappa le coordinate
            unsigned orgX = x * orgSize.x / lato;
            unsigned orgY = y * orgSize.y / lato;
            ridimensionata.setPixel(x, y, originale.getPixel(orgX, orgY));
        }
    }
    return ridimensionata;
}

// trasforma il vettore in un'immagine visibile
void Pattern::display() const {
    sf::Image immagineVisiva;
    immagineVisiva.create(lato, lato); 

    for (unsigned int i = 0; i < numNeurons; i++) {
        unsigned int x = i % lato;
        unsigned int y = i / lato;
        
        // Uso directly this->neurons
        int valore = neurons[i];

        if (valore == 1) {
            immagineVisiva.setPixel(x, y, sf::Color::White); // Attivo
        } else {
            immagineVisiva.setPixel(x, y, sf::Color::Black); // Spento (-1)
        }
    }

    sf::Texture texture;
    texture.loadFromImage(immagineVisiva);
    sf::Sprite sprite(texture);
    sprite.setScale(10.0f, 10.0f); 

    sf::RenderWindow window(sf::VideoMode(lato * 10, lato * 10), "Pattern Preview");

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

// Funzione Principale: Carica, Ridimensiona e Converte
bool Pattern::loadFromImage(const std::string& imgName) {
    sf::Image immagineGrezza;
    if (!immagineGrezza.loadFromFile("./images/" + imgName)) {
        std::cerr << "Errore: Impossibile caricare il file " << imgName << std::endl;
        return false; 
    }

    sf::Image immagine = Pattern::resize(immagineGrezza);

    neurons.clear();
    neurons.reserve(numNeurons);

    for (unsigned int y = 0; y < lato; y++) {
        for (unsigned int x = 0; x < lato; x++) {
            sf::Color colore = immagine.getPixel(x, y);
            int luminosita = (static_cast<int>(colore.r) + static_cast<int>(colore.g) + static_cast<int>(colore.b)) / 3;

            // Logica Bipolare: >127 è +1, altrimenti -1
            if (luminosita > 127) 
                neurons.push_back(1); 
            else 
                neurons.push_back(-1);
        }
    }

    /*unsigned int width = immagine.getSize().x;
    unsigned int height = immagine.getSize().y;

    std::vector<int> input_rete;
    input_rete.reserve(width * height);

    // conversione in -1 / +1
    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            sf::Color colore = immagine.getPixel(x, y);
            int luminosita = (static_cast<int>(colore.r) + static_cast<int>(colore.g) + static_cast<int>(colore.b)) / 3;

            if (luminosita > 127) input_rete.push_back(1); 
            else input_rete.push_back(-1);
        }
    } era il vecchio metodo */

    // display(); in automatico o la chiamiamo?

    return true;
}