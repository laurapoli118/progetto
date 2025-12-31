#include "pattern.hpp"
#include <iostream> // Serve per stampare gli errori (cerr)
#include <cstdlib>  // Serve per la funzione exit(1)
#include <vector>
#include <cmath>
#include <fstream>


//RICORDARSI DI METTERE TUTTE LE ASSERT E EXCEPTION PER GESTIRE RUNTIME


Pattern::Pattern(int size)
{

    height = size;
    base = size;
    numNeurons = size * size;
    neurons.resize(numNeurons);
}

void Pattern::setNeuron(unsigned index, int value)
{

    neurons[index] = value;
}

unsigned Pattern::getBase() const
{

    return base;
}

unsigned Pattern::getHeight() const
{

    return height;
}

unsigned Pattern::getNumNeurons() const
{

    return numNeurons;
}


