#include "rete_neurale/acquisition.hpp"
#include "rete_neurale/matrix.hpp"
#include "rete_neurale/pattern.hpp"
#include "rete_neurale/userInterface.hpp"
#include <cassert>


int main() {

  std::cout << "Hopfield Network Creation.\n";
  std::srand(static_cast<unsigned>(std::time(nullptr)));
  std::cout << "Insert the number of pixels on the side of the image you want to use (32~128): ";
  unsigned lato{};
  std::cin >> lato;
  assert(32 <= lato && lato <= 128 && "Image size is too low or too high."); 
  hp::Matrix matrix(lato);
  hp::TrainingPhase(matrix, lato);
  hp::RecallPhase(matrix, lato);
  
  return 0;
}