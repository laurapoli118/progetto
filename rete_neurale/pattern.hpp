#include <vector>   
#include <string> 


class Pattern
{
private:
    unsigned height;
    unsigned base;
    unsigned numNeurons;
    std::vector<int> neurons;

public:
    Pattern(int size);

    // Funzioni per settare valori

    void setSpin(unsigned index, int value);

    // Funzioni per ottenere le grandezze del pattern

    int getSpin(unsigned index) const;
    unsigned getHeight() const;
    unsigned getBase() const;
    unsigned getNumNeurons() const;
    const std::vector<int> &getData() const;

    // Funzione per rumore

    void addNoise(float noisePerc);

    // Gestione e salvataggio pattern

    void saveToFile(const std::string &filename) const;
    void loadFromFile(const std::string &filename);
};
