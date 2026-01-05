#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <fstream>
#include <random>
#include <algorithm>
#include <fstream>
#include <string>
#include <train.hpp>
#include <pattern.hpp>

Train::Train(unsigned size) // la inizializzo a 0 e poi sommo i pesi
{

    numNeurons = size * size;

    for (unsigned i = 0; i < numNeurons; ++i)
    {
        std::vector<float> row; // inizializza un vettore per ogni riga
        for (unsigned j = 0; j < numNeurons; ++j)
        {
            row.push_back(0.0f);
        }
        weights.push_back(row);
    }
}

float Train::getWeight(unsigned i, unsigned j) const
{

    return weights[i][j];
}

void Train::setWeight(unsigned i, unsigned j, float weight)
{
    weights[i][j] = weight;
}

void Train::learnPattern(const Pattern &pattern) // così forse è un po troppo messa bene, se vogliamo essere più grulli
{
    float normFactor = 1.0f / numNeurons;

    for (unsigned i = 0; i < numNeurons; ++i)
    {
        for (unsigned j = 0; j < numNeurons; ++j)
        {

            if (i != j) // controllo per la diagonale nulla
            {
                float coeffWeight = pattern.getNeuron(i) * pattern.getNeuron(j) * normFactor;

                weights[i][j] += coeffWeight;
            }
        }
    }
}
