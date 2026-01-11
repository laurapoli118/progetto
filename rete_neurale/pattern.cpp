#include "pattern.hpp"
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
// RICORDARSI DI METTERE TUTTE LE ASSERT E EXCEPTION PER GESTIRE RUNTIME

Pattern::Pattern(int size)
{

    size_ = size;
    numNeurons_ = size * size;
    neurons_.resize(numNeurons_); // questo è da capire se si può toglierere
}

// FUNZIONI PER SETTARE

void Pattern::setNeuron(unsigned index, int value)
{
    assert(index < numNeurons_);

    neurons_[index] = value;
}

// FUNZIONI GETTERS

int Pattern::getNeuron(unsigned index) const
{
    assert(index < numNeurons_);
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

const std::vector<int> &Pattern::getData() const
{
    return neurons_;
}

// FUNZIONE NOISE

void Pattern::addNoise(float noisePerc) // QUA HO USATO UNA LAMBDA E UN ALGORITMO,
{                                       // VOLENDO SI PUO' FARE ANCHE CON UN CICLO FOR

    // std::transform prende: Inizio, Fine, Dove Scrivere, La Funzione da applicare
    std::transform(neurons_.begin(), neurons_.end(), neurons_.begin(), [=](int currentNeuron)
                   {


        float random = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);

        if (random < noisePerc)
        
            {
                return -currentNeuron;
            }
            else
            {
                return currentNeuron;
            } });
}
