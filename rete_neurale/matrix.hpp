#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include "pattern.hpp"

class Matrix {

private:
    std::vector<std::vector<float>> weights_; // da decidere come la vogliamo fare
    unsigned numNeurons_;
    std::vector<std::vector<int>> newPattern;

public:
    Matrix(unsigned size);
    float getWeight(unsigned i, unsigned j) const;
    void setWeight(unsigned i, unsigned j, float weight);
    void learnPattern(const Pattern &pattern);
    void recall(Pattern &pattern);
};

#endif