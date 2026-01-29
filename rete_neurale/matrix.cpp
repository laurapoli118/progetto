#include "matrix.hpp"
#include "pattern.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <stdexcept>

Matrix::Matrix(unsigned size)
{ if (size == 0) {
      throw std::invalid_argument("Errore: la dimensione della matrice non può essere zero!");
  }
  // la inizializzo a 0 e poi sommo i pesi
  numNeurons_ = size * size;
  // invece di fare tanti push_back, ridimensioniamo subito la matrice
  // vettore di numNeurons_ righe, ognuna contenente un vettore di numNeurons_
  // zeri
  weights_.resize(numNeurons_, std::vector<float>(numNeurons_, 0.0f));
}

float Matrix::getWeight(unsigned i, unsigned j) const
{
  assert(i < numNeurons_ && j < numNeurons_); // assert aggiunto da ettore tramite gemini
  return weights_[i][j];
}

void Matrix::setWeight(unsigned i, unsigned j, float weight)
{
  assert(i < numNeurons_ && j < numNeurons_); // assert aggiunto da ettore tramite gemini
  weights_[i][j] = weight;
}

float Matrix::calcEnergy(const Pattern& pattern) const
{
  float energy = 0.0f;
  for (unsigned i = 0; i < numNeurons_; ++i) {
    for (unsigned j = 0; j < numNeurons_; ++j) {
      energy +=
          -0.5f * getWeight(i, j) * pattern.getNeuron(i) * pattern.getNeuron(j);
    }
  }
  return energy;
}

void Matrix::learnPattern(const Pattern& pattern)
{ // CONTROLLO DIMENSIONE: Se il pattern non ha lo stesso numero di neuroni della matrice, STOP!
  if (pattern.getNumNeurons() != numNeurons_) {
      throw std::runtime_error("Errore: La dimensione del pattern non corrisponde con la dimensione del lato della matrice!");
  }
  
  // così forse è un po troppo messa bene, se vogliamo essere più grulli
  float normFactor = 1.0f / numNeurons_;
  for (unsigned i = 0; i < numNeurons_; ++i) {
    for (unsigned j = 0; j < numNeurons_; ++j) {
      if (i != j) { // controllo per la diagonale nulla
        float coeffWeight = pattern.getNeuron(i) * pattern.getNeuron(j)
                          * normFactor; // SE TOGLIESSIMO LA VARIABILE IN PIù E
                                        // METTESSIMO /NUMNEURONS_? dopo è meno efficiente
        weights_[i][j] += coeffWeight;
      }
    }
  }
}
/*
void Matrix::recall(Pattern& pattern)
{
  bool conv               = false;
  unsigned int maxRuns    = 100;
  unsigned int currentRun = 1;
  std::vector<float> energies;
  energies.push_back(calcEnergy(pattern)); // energia iniziale

  while (!conv && currentRun <= maxRuns) {
    std::vector<int> currentPattern =
        pattern.getData(); // nuovo vettore per verificare converga
    std::vector<int> newPattern =
        currentPattern;     // HO MESSO DOUBLE BUFFERING (due vettori) così che
                            // durante un giro gli ultimi cambi non siano
                            // influenzati dai primi
    unsigned convCheck = 0; // variabile per verificare se per ogni i è uguale

    for (unsigned i = 0; i < numNeurons_; i++) {
      double sum = 0.0;
      for (unsigned j = 0; j < numNeurons_; j++) {
        sum += weights_[i][j] * currentPattern[j];
      }
      if (sum > 0) {
        newPattern[i] = 1;
      } else {
        newPattern[i] = -1;
      }
      if (newPattern[i] != currentPattern[i]) {
        convCheck++;
      }
    }
    for (unsigned k = 0; k < numNeurons_;
         k++) { // modifichiamo effettivamente l'oggetto nuovo
      pattern.setNeuron(k, newPattern[k]);
    }

    energies.push_back(calcEnergy(pattern)); // energia ad ogni step

    if (convCheck == 0) {
      conv = true;
    }

    currentRun++;
  }
  assert(currentRun != maxRuns
         && "Err: Reached maxRuns without perfect convergence.");

  for (unsigned i = 0; i < energies.size(); i++) {
    std::cout << "Energy at step " << i << ": " << energies[i] << std::endl;
  }
}
*/

std::vector<float> Matrix::recall(Pattern& pattern) 
{
  unsigned int maxRuns= 1500;
  unsigned int currentRun=1;
  float temp=2.0f;
  float minTemp = 0.01f;
  float alpha=0.98f;

  static std::random_device rd; // con static llo crea una volta sola per tutto il programma
  static std::mt19937 gen(rd());
  static std::uniform_real_distribution<float> dis(0.0f, 1.0f); 
  std::uniform_int_distribution<int> randNeuron(0, numNeurons_ - 1); // Per scegliere neurone a caso
  // perchè gemini dice che il vero Simulated Annealing funziona su neuroni casuali non in fila
  std::vector<float> energyHistory;
  float currentEnergy = calcEnergy(pattern);
  energyHistory.push_back(currentEnergy); // energia iniziale

  while(temp>minTemp && currentRun<= maxRuns){
    unsigned int changesThisRun=0; 
    for(unsigned k=0; k < numNeurons_; k++) {
      unsigned int i = randNeuron(gen);

      float localField=0.0;
      for(unsigned j=0; j< numNeurons_; j++){
        // ho spiegato a gemini che non serve togliere il caso i=j perchè weights (i)(i) è 0 e si è stupito 
        localField += weights_[i][j] * pattern.getNeuron(j);
      }

      double deEnergy=2.0 * localField * pattern.getNeuron(i);
      // float prob=std::exp(-deEnergy/temp);
      if (deEnergy < 0 || dis(gen) < std::exp(-deEnergy/temp)) {
        pattern.setNeuron (i, -pattern.getNeuron(i)); //flip se fa diminuire l'energia o se la temperatura è abbastanza alta da acceettare la mossa sbagliata
        currentEnergy += deEnergy;
        changesThisRun++;
    }
  }
    
    energyHistory.push_back(currentEnergy); // TOLTo IL RICALCOLO PERCHè è MOOLTO PIù VELOCE SEMPLICEMENTE AGGIUNGERE DEENERGY

    temp*=alpha; // a ogni step si riduce la temperatura variando prob
    currentRun++;
    
    if(temp <0.05f && changesThisRun==0 ) { // esce prima se temp è bassa e non ha cambiato niente
      break;
    }
  }
  /* for (unsigned i=0; i<energies.size(); i++){
    std::cout << "step"<< i <<":"<<energies[i]<< std::endl;
  } 
    lo metterei nel main così:
   std::vector<float> storia = matrix.recall(dirty);

std::cout << "Energia Iniziale: " << storia.front() << "\n";
std::cout << "Energia Finale:   " << storia.back() << "\n";
std::cout << "Step totali:      " << storia.size() << "\n";*/
  
  return energyHistory;
}