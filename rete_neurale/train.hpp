#ifndef TRAIN_HPP
#define TRAIN_HPP

#include <vector>
#include "pattern.hpp"

class Train
{

private:
    std::vector<std::vector<float>> weights_; // da decidere come la vogliamo fare
    unsigned numNeurons_;
    std::vector<std::vector<int>> newPattern;

public:
    Train(unsigned size);

    float getWeight(unsigned i, unsigned j) const;

    void setWeight(unsigned i, unsigned j, float weight);

    void learnPattern(const Pattern &pattern);

     void recall(const Pattern &pattern);
};

#endif