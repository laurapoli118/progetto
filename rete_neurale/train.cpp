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
#include "pattern.hpp"

Train::Train(unsigned size)
{

    numNeurons = size * size;

    for (unsigned i = 0; i < numNeurons; ++i)
    {
        std::vector<float> row;
        for (unsigned j = 0; j < numNeurons; ++j)
        {
            row.push_back(0.0f);
        }
        weights.push_back(row);
    }
}



float Train::getWeight(unsigned i, unsigned j) const{

    return weights[i][j];
}

void Train::setWeight(unsigned i, unsigned j, float weight)
{
    weights[i][j] = weight;
}

void learnPattern(const Pattern& pattern){

    for(unsigned i=0; i<numNeurons; ++i){
        for(unsigned j=0; j<numNeurons; ++j){
            
        
            }
        }
    }



}


