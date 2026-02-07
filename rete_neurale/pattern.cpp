#include "pattern.hpp"
#include <algorithm>
#include <cassert>
#include <random>
#include <stdexcept>
#include <vector>

namespace hp {

Pattern::Pattern(unsigned size)

{
  assert(size != 0 && "Error: size must be greater than 0.");
  size_       = size;
  numNeurons_ = size * size;

  neurons_.resize(numNeurons_, 0);
}

void Pattern::setNeuron(unsigned index, int value)
{
  assert(index < numNeurons_ && "Error: can't set an inexisting nueron.");
  neurons_[index] = value;
}

int Pattern::getNeuron(unsigned index) const
{
  assert(index < numNeurons_ && "Error: can't get an inexisting nueron.");
  return neurons_[index];
}

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
        "Error: noise percentage must be between 0 and 1.");
  }
  static std::random_device rd;   // per ottenere un seed casuale
  static std::mt19937 gen(rd());   // motore algoritmico
  std::uniform_real_distribution<float> dis(0.0f, 1.0f);

  for (int& neuron : neurons_) {
    if (dis(gen) < noisePerc) {
      neuron = -neuron;
    }
  }
}

bool Pattern::isIdentical(const Pattern& current) const
{
  assert(getNumNeurons() == current.getNumNeurons()
         && "Error: Patterns must have the same number of neurons to compare.");

  const std::vector<int>& currentData = current.getData();
  if (neurons_ == currentData) {
    return true;
  }

  bool isInverted =
      std::equal(neurons_.begin(), neurons_.end(), currentData.begin(),  //equal(partenza, fine, dove opera, funzione che dice se sono uguali)
                 [](int a, int b) { return a == -b; });

  return isInverted;
}

} // namespace hp
