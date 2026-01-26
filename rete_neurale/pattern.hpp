#ifndef PATTERN_HPP
#define PATTERN_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>

class Pattern
{
 private:
  unsigned size_;
  unsigned numNeurons_;
  std::vector<int> neurons_;
  

 public:
  Pattern(int size);  // costruttore

  // Funzioni per settare valori

  void setNeuron(unsigned index, int value);

  // Funzioni per ottenere le grandezze del pattern

  int getNeuron(unsigned index) const;

  unsigned getSize() const;

  unsigned getNumNeurons() const;

  const std::vector<int>& getData() const;

  // Funzione per rumore

  void addNoise(float noisePerc);

  bool checkConv(const Pattern& current) const;

  
  
};

#endif