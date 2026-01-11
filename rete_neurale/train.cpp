#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <fstream>
#include <random>
#include <algorithm>
#include <fstream>
#include <string>
#include "train.hpp"
#include "pattern.hpp"

Train::Train(unsigned size) // la inizializzo a 0 e poi sommo i pesi
{

    numNeurons_ = size * size;

    for (unsigned i = 0; i < numNeurons_; ++i)
    {
        std::vector<float> row; // inizializza un vettore per ogni riga
        for (unsigned j = 0; j < numNeurons_; ++j)
        {
            row.push_back(0.0f);
        }
        weights_.push_back(row);
    }
}

float Train::getWeight(unsigned i, unsigned j) const
{

    return weights_[i][j];
}

void Train::setWeight(unsigned i, unsigned j, float weight)
{
    weights_[i][j] = weight;
}

void Train::learnPattern(const Pattern &pattern) // così forse è un po troppo messa bene, se vogliamo essere più grulli
{
    float normFactor = 1.0f / numNeurons_;

    for (unsigned i = 0; i < numNeurons_; ++i)
    {
        for (unsigned j = 0; j < numNeurons_; ++j)
        {

            if (i != j) // controllo per la diagonale nulla
            {
                float coeffWeight = pattern.getNeuron(i) * pattern.getNeuron(j) * normFactor;

                weights_[i][j] += coeffWeight;
            }
        }
    }
}

void Train::recall(const Pattern &pattern) {
    bool conv = false;
    while(!conv) {
        std::vector<int> newPattern(numNeurons_); //nuovo vettore per verificare converga
        unsigned convCheck=0; //variabile per verificare se per ogni i è uguale
        for(unsigned i=0; i < numNeurons_; i++) {
            double sum = 0.0;
            for(unsigned j=0; j < numNeurons_; j++) {
                sum += weights_[i][j] * pattern.getNeuron(j);
            }
            if(sum > 0) {
                newPattern[1]=1;
            } else {
                newPattern[i]=-1;
            }
            if(newPattern[i]==pattern.getNeuron[i]) {
                convCheck ++;
            }
        }
        if(convCheck == numNeurons_) {
            conv= true;
        }
    }
}