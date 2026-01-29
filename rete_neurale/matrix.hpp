#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include "pattern.hpp"

class Matrix {

private:
    std::vector<std::vector<float>> weights_; // da decidere come la vogliamo fare
    unsigned numNeurons_;
    std::vector<Pattern> storedPatterns_; //rega questo serve per non fare fallire il test di riconoscimento pattern noto
    // std::vector<std::vector<int>> newPattern; TOLTO PERCHè NON PIù USATO MI PARE

public:
    Matrix(unsigned size);
    float getWeight(unsigned i, unsigned j) const;
    void setWeight(unsigned i, unsigned j, float weight);
    void learnPattern(const Pattern &pattern);
    std::vector<float> recall(Pattern &pattern);
    float calcEnergy(const Pattern &pattern) const;
};

#endif