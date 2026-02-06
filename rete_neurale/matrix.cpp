#include "matrix.hpp"
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
namespace hp {
Matrix::Matrix(unsigned size)
{
  if (size == 0) {
    throw std::invalid_argument(
        "Errore: la dimensione della matrice non può essere zero!");
  }

  numNeurons_ = size * size;

  weights_.resize(numNeurons_, std::vector<float>(numNeurons_, 0.0f));
}

float Matrix::getWeight(unsigned i, unsigned j) const
{
  assert(i < numNeurons_ && j < numNeurons_);
  return weights_[i][j];
}

void Matrix::setWeight(unsigned i, unsigned j, float weight)
{
  assert(i < numNeurons_ && j < numNeurons_);
  weights_[i][j] = weight;
}

float Matrix::calcEnergy(const Pattern& pattern) const
{
  float energy = 0.0f;
  for (unsigned i = 0; i < numNeurons_; ++i) {
    for (unsigned j = 0; j < numNeurons_; ++j) {
      energy += -0.5f * getWeight(i, j)
              * static_cast<float>(pattern.getNeuron(i))
              * static_cast<float>(pattern.getNeuron(j));
    }
  }
  return energy;
}

bool Matrix::learnPattern(const Pattern& pattern)
{
  if (pattern.getNumNeurons() != numNeurons_) {
    throw std::runtime_error(
        "Errore: La dimensione del pattern non corrisponde con la dimensione "
        "del lato della matrice!");
  }

  for (const auto& memory : storedPatterns_) {
    if (pattern.isIdentical(memory)) {
      std::cout << "Image already learnt!\n";
      return false;
    }
  }

  storedPatterns_.push_back(pattern);

  /* float normFactor = 1.0f / static_cast<float>(numNeurons_);
  for (unsigned i = 0; i < numNeurons_; ++i) {
    for (unsigned j = 0; j < numNeurons_; ++j) {
      if (i != j) {
        float coeffWeight = static_cast<float>(pattern.getNeuron(i))
                          * static_cast<float>(pattern.getNeuron(j))
                          * normFactor;
        weights_[i][j] += coeffWeight;
      }
    }
  }
  return true; */

  // --- INIZIO ALGORITMO DI STORKEY ---

  // PASSO A: Calcolo il "Campo Locale" (h)
  // Misuriamo quanto i ricordi vecchi "interferiscono" con il pattern nuovo.
  // Se è la prima immagine, i pesi sono 0, quindi h sarà tutto 0 (diventa Hebb puro).
  std::vector<float> h(numNeurons_, 0.0f);
  
  for (unsigned i = 0; i < numNeurons_; ++i) {
      for (unsigned j = 0; j < numNeurons_; ++j) {
          if (i != j) {
             // h[i] += peso_esistente * valore_nuovo_pixel
             h[i] += weights_[i][j] * static_cast<float>(pattern.getNeuron(j));
          }
      }
  }

  // PASSO B: Aggiornamento dei Pesi con la correzione
  // Formula: DeltaW_ij = (1/N) * [ xi*xj  -  (1/N)*(xi*h_j + h_i*xj) ]
  //                       ^Hebb     ^Correzione Storkey
  
  float n = static_cast<float>(numNeurons_); // N
  float invN = 1.0f / n;                     // 1/N

  for (unsigned i = 0; i < numNeurons_; ++i) {
    for (unsigned j = 0; j < numNeurons_; ++j) {
      if (i != j) {
        float p_i = static_cast<float>(pattern.getNeuron(i));
        float p_j = static_cast<float>(pattern.getNeuron(j));

        // 1. Parte Hebbiana classica (Attrazione)
        float hebbTerm = p_i * p_j;

        // 2. Parte Storkey (Correzione dell'interferenza)
        // Sottrae l'influenza del campo locale
        float correctionTerm = (p_i * h[j] + h[i] * p_j) * invN;

        // 3. Calcolo finale del peso da aggiungere
        float deltaWeight = (hebbTerm - correctionTerm) * invN;

        weights_[i][j] += deltaWeight;
      }
    }
  }

  return true;
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
  std::vector<float> energyHistory;
  float currentEnergy = calcEnergy(pattern);
  energyHistory.push_back(currentEnergy); // energia iniziale

  for (const auto& memory : storedPatterns_) {
    if (pattern.isIdentical(memory)) {
      std::cout << "This image isn't corrupted!\n";
      return energyHistory;
    }
  }

  unsigned int maxRuns    = 1000;
  unsigned int currentRun = 1;
  float temp              = 0.2f;
  float minTemp           = 0.05f;
  float alpha             = 0.96f;

  bool doAnnealing = true;

  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_real_distribution<float> dis(0.0f, 1.0f);
  std::uniform_int_distribution<int> randNeuron(0, numNeurons_ - 1);
  std::vector<int> neuronIndices(numNeurons_);
  std::iota(neuronIndices.begin(), neuronIndices.end(), 0);

  std::cout << "Run: 0, Energy: " << currentEnergy << '\n';

  while (currentRun <= maxRuns) {
    unsigned int changesThisRun = 0;

    if (doAnnealing) {
      std::shuffle(neuronIndices.begin(), neuronIndices.end(), gen);
    }

    for (int i : neuronIndices) {
      float localField = 0.0f;
      for (unsigned j = 0; j < numNeurons_; j++) {
        localField +=
            getWeight(i, j) * static_cast<float>(pattern.getNeuron(j));
      }

      double deEnergy = 2.0 * localField * pattern.getNeuron(i);
      bool doFlip     = false;

      if (deEnergy < 0) {
        doFlip = true;
      } else if (doAnnealing && dis(gen) < std::exp(-deEnergy / temp)) {
        doFlip = true;
      }
      if (doFlip) {
        pattern.setNeuron(i, -pattern.getNeuron(i));
        currentEnergy += static_cast<float>(deEnergy);
        changesThisRun++;
      }
    }
    std::cout << "Run: " << currentRun << ", Neurons changed: " << changesThisRun << ", Temp: " << temp
              << ", Energy: " << currentEnergy << '\n';
    energyHistory.push_back(currentEnergy);

    if (doAnnealing) {
      temp *= alpha;
      if (temp < minTemp) {
        doAnnealing = false;
      }
    } else if (changesThisRun == 0) {
      break;
    }
    currentRun++;
  }
  return energyHistory;
}

} // namespace hp