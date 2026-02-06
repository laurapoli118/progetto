#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "pattern.hpp"
#include <vector>
namespace hp {
class Matrix
{
 private:
  std::vector<std::vector<float>> weights_;
  unsigned numNeurons_;
  std::vector<Pattern> storedPatterns_;

 public:
  Matrix(unsigned size);
  float getWeight(unsigned i, unsigned j) const;
  void setWeight(unsigned i, unsigned j, float weight);
  bool learnPattern(const Pattern& pattern);
  std::vector<float> recall(Pattern& pattern);
  float calcEnergy(const Pattern& pattern) const;
};
} 
#endif