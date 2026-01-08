#ifndef PATTERN_HPP
#define PATTERN_HPP

#include <vector>
#include <string>

class Pattern
{
private:
    unsigned lato;       // qua bisogna decidere se usare solo un lato o base e latezza
    unsigned numNeurons; // comunqur ci conviene lavorare con immagini quadrate così è più semplice
    std::vector<int> neurons;

public:
    Pattern(int size);

    // Funzioni per settare valori

    void setNeuron(unsigned index, int value);

    // Funzioni per ottenere le grandezze del pattern

    int getNeuron(unsigned index) const;

    unsigned getLato() const;

    unsigned getNumNeurons() const;

    const std::vector<int> &getData() const;

    // Funzione per rumore

    void addNoise(float noisePerc);

    // Gestione e salvataggio pattern   QUESTE SERVONO SOLTANTO SE VOGLIAMO FARE 3 ESEGUIBILI
    // SEPARATI O SE VOGLIAMO SALVARE UN FILE CON TUTTI GLI INGRESSI DEI VETTORI
    // void saveToFile(const std::string &filename) const;

    // void loadFromFile(const std::string &filename);
};

#endif