#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <fstream>
#include <random>
#include <algorithm>
#include <fstream>
#include <string>
#include <train.hpp>

Train::Train(unsigned lato)
{

    numNeurons = lato * lato;

    for (unsigned i = 0; i < numNeurons; ++i)
    {
        std::vector<float> row;
        for (unsigned j = 0; j < numNeurons; ++j)
        {
            row.push_back(0.0f);
        }
        weights.push_back(row);
    }
}
