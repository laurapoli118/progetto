#ifndef PATTERN_HPP
#define PATTERN_HPP

#include <iostream>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

class Pattern
{
private:
    unsigned size_;       // qua bisogna decidere se usare solo un lato o base e latezza
    unsigned numNeurons_; // comunqur ci conviene lavorare con immagini quadrate così è più semplice
    std::vector<int> neurons_;
    sf::Image resize(const sf::Image& originale) const;

public:
    Pattern(int size);

    // Funzioni per settare valori

    void setNeuron(unsigned index, int value);

    // Funzioni per ottenere le grandezze del pattern

    unsigned getLato() const;

    int getNeuron(unsigned index) const;

    unsigned getSize() const;

    unsigned getNumNeurons() const;

    const std::vector<int> &getData() const;

    // Funzione per rumore

    void addNoise(float noisePerc);

    // Gestione e salvataggio pattern   QUESTE SERVONO SOLTANTO SE VOGLIAMO FARE 3 ESEGUIBILI
    // SEPARATI O SE VOGLIAMO SALVARE UN FILE CON TUTTI GLI INGRESSI DEI VETTORI
    // void saveToFile(const std::string &filename) const;

    // void loadFromFile(const std::string &filename);

    bool loadFromImage(const std::string& filename);
    void display() const;
};

#endif