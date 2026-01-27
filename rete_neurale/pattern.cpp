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

// RICORDARSI DI AGGIUNGERE ASSERT E EXCEPTION PER GESTIRE RUNTIME

#ifndef ROOT_DIR
#  define ROOT_DIR "."
#endif

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
{ // POI LOLLO QUANDO VUOI MI SPIEGHI STA MERDA E VALUTIAMO QUALE FARE

  // CONTROLLO INPUT: La percentuale deve essere tra 0.0 (0%) e 1.0 (100%)
  if (noisePerc < 0.0f || noisePerc > 1.0f) {
      throw std::invalid_argument("Errore: la percentuale di rumore deve essere compresa tra 0 e 100");
  }
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

      });
}

bool Pattern::checkConv( const Pattern& current) const
{
  assert(getNumNeurons() == current.getNumNeurons() &&
         "Error: Patterns must have the same number of neurons to compare.");
  unsigned n = getNumNeurons();

  for (unsigned i = 0; i < n; ++i) {
    if (getNeuron(i) != current.getNeuron(i)) {
      return false;
    }
  }
  return true;
}
