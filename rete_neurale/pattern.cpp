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

// RICORDARSI DI METTERE TUTTE LE ASSERT E EXCEPTION PER GESTIRE RUNTIME

Pattern::Pattern(int size)
{

    lato = size;
    numNeurons = size * size;
    neurons.resize(numNeurons);  //questo è da capire se si può toglire
}

// FUNZIONI PER SETTARE

void Pattern::setNeuron(unsigned index, int value)
{

    neurons[index] = value;
}

// FUNZIONI GETTERS

int Pattern::getNeuron(unsigned index) const
{

   return neurons[index];

}



unsigned Pattern::getLato() const
{

    return lato;
}

unsigned Pattern::getNumNeurons() const
{

    return numNeurons;
}

const std::vector<int> &Pattern::getData() const
{
    return neurons;
}

// FUNZIONE NOISE

void Pattern::addNoise(float noisePerc) // QUA HO USATO UNA LAMBDA E UN ALGORITMO,
{                                       // VOLENDO SI PUO' FARE ANCHE CON UN CICLO FOR

    // std::transform prende: Inizio, Fine, Dove Scrivere, La Funzione da applicare
    std::transform(neurons.begin(), neurons.end(), neurons.begin(), [=](int currentNeuron)
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
