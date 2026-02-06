#ifndef PATTERN_HPP
#define PATTERN_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>

namespace hp {
class Pattern
{
 private:
  unsigned size_;
  unsigned numNeurons_;
  std::vector<int> neurons_;

 public:
  Pattern(int size);
  void setNeuron(unsigned index, int value);
  int getNeuron(unsigned index) const;
  unsigned getSize() const;
  unsigned getNumNeurons() const;
  const std::vector<int>& getData() const;
  void addNoise(float noisePerc);
  bool isIdentical(const Pattern& current) const;
};
} // namespace hp

#endif