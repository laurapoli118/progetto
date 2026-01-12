#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <fstream>
#include <random>
#include <algorithm>
#include <fstream>
#include <string>
#include <cassert>
#include "matrix.hpp"
#include "pattern.hpp"

Matrix::Matrix(unsigned size) { // la inizializzo a 0 e poi sommo i pesi
    numNeurons_ = size * size;
    // invece di fare tanti push_back, ridimensioniamo subito la matrice
    // vettore di numNeurons_ righe, ognuna contenente un vettore di numNeurons_ zeri
    weights_.resize(numNeurons_, std::vector<float>(numNeurons_, 0.0f));
}

float Matrix::getWeight(unsigned i, unsigned j) const {
    return weights_[i][j];
}

void Matrix::setWeight(unsigned i, unsigned j, float weight) {
    weights_[i][j] = weight;
}

void Matrix::learnPattern(const Pattern &pattern) { // così forse è un po troppo messa bene, se vogliamo essere più grulli
    float normFactor = 1.0f / numNeurons_;
    for (unsigned i = 0; i < numNeurons_; ++i) {
        for (unsigned j = 0; j < numNeurons_; ++j) {
            if (i != j) { // controllo per la diagonale nulla
                float coeffWeight = pattern.getNeuron(i) * pattern.getNeuron(j) * normFactor; // SE TOGLIESSIMO LA VARIABILE IN PIù E METTESSIMO /NUMNEURONS_?
                weights_[i][j] += coeffWeight;
            }
        }
    }
}

void Matrix::recall(Pattern &pattern) {
    bool conv = false;
    unsigned int maxRuns = 100;
    unsigned int currentRun = 0;
    while(!conv && currentRun <= maxRuns) {
        std::vector<int> currentPattern = pattern.getData();       // nuovo vettore per verificare converga
        std::vector<int> newPattern = currentPattern;           // HO MESSO DOUBLE BUFFERING (due vettori) così che durante un giro gli ultimi cambi non siano influenzati dai primi
        unsigned convCheck = 0;                           // variabile per verificare se per ogni i è uguale
        for(unsigned i=0; i < numNeurons_; i++) {
            double sum = 0.0;
            for(unsigned j=0; j < numNeurons_; j++) {
                sum += weights_[i][j] * currentPattern[j];
            }
            if(sum > 0) {
                newPattern[i] = 1;
            } else {
                newPattern[i] = -1;
            }
            if(newPattern[i] != currentPattern[i]) {
                convCheck ++;
            }
        }
        for(unsigned k=0; k < numNeurons_; k++) { // modifichiamo effettivamente l'oggetto nuovo
            pattern.setNeuron(k, newPattern[k]);
        }
        if(convCheck == 0) {
            conv = true;
        }
        currentRun++;
    }
    assert(currentRun == maxRuns && "Err: Reached maxRuns without perfect convergence.");
}