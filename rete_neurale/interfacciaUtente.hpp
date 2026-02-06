#ifndef USERINTERFACE_HPP
#define USERINTERFACE_HPP

#include "acquisition.hpp"
#include "matrix.hpp"
#include "pattern.hpp"

namespace hp {

void TrainingPhase(hp::Matrix& matrix, unsigned lato);
void RecallPhase(hp::Matrix& matrix, unsigned lato);
}

#endif
