#include "pattern.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>
namespace hp{
Pattern::Pattern(int size)
    : size_(size)
    , numNeurons_(size * size)
{
  assert(size > 0 && "Error: size must be greater than 0.");
  neurons_.resize(numNeurons_, 0); // costruttore che inizializza il vettore con
                                   // zeri, vedi giù per diff. resize e reserve
}

void Pattern::setNeuron(unsigned index, int value)
{
  // FUNZIONE SETTER
  assert(index <= numNeurons_ && "Error: can't set an inexisting nueron.");
  neurons_[index] = value;
}

int Pattern::getNeuron(unsigned index) const
{
  return neurons_[index];
}

// FUNZIONI GETTERS

unsigned Pattern::getSize() const
{
  return size_;
}

unsigned Pattern::getNumNeurons() const
{
  return numNeurons_;
}

const std::vector<int>& Pattern::getData() const
{
  return neurons_;
}

void Pattern::addNoise(float noisePerc)
{
  
  if (noisePerc < 0.0f || noisePerc > 1.0f) {
    throw std::invalid_argument(
        "Errore: la percentuale di rumore deve essere compresa tra 0 e 100"); // potremmo mettere un catch??
  }

  static std::random_device rd;  
  static std::mt19937 gen(rd()); 
  

  std::uniform_real_distribution<float> dis(0.0f, 1.0f); 
  for (int& neuron : neurons_) {
        if (dis(gen) < noisePerc) {
            neuron = -neuron; 
        }
      }
  /*
  std::transform(
      neurons_.begin(), neurons_.end(), neurons_.begin(),
      [=](int currentNeuron) { // std::transform prende: Inizio, Fine, Dove
                               // Scrivere, La Funzione da applicare
        float random =
            static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
        if (random < noisePerc) {
          return -currentNeuron;
        } else {
          return currentNeuron;
        }

      }); */
}

bool Pattern::isIdentical(const Pattern& current) const {
  assert(getNumNeurons() == current.getNumNeurons()
         && "Error: Patterns must have the same number of neurons to compare.");

  const std::vector<int>& otherData = current.getData();
  if (neurons_ == otherData) {
        return true;
    }

  bool isInverted = std::equal(neurons_.begin(), neurons_.end(), otherData.begin(), [](int a, int b) {
    return a == -b; 
    });

  return isInverted;
}
}
