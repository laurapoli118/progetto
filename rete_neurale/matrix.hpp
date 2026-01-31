#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include "pattern.hpp"
namespace hp{
class Matrix {

private:
    std::vector<std::vector<float>> weights_; // da decidere come la vogliamo fare
    unsigned numNeurons_;
    std::vector<hp::Pattern> storedPatterns_; //rega questo serve per non fare fallire il test di riconoscimento pattern noto
    // std::vector<std::vector<int>> newPattern; TOLTO PERCHè NON PIù USATO MI PARE

public:
    Matrix(unsigned size);
    float getWeight(unsigned i, unsigned j) const;
    void setWeight(unsigned i, unsigned j, float weight);
    void learnPattern(const hp::Pattern &pattern);
    std::vector<float> recall(hp::Pattern &pattern);
    float calcEnergy(const hp::Pattern &pattern) const;
};
}
#endif