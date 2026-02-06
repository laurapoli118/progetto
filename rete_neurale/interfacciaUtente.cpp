#include "interfacciaUtente.hpp"
#include "acquisition.hpp"
#include "matrix.hpp"
#include "pattern.hpp"
#include <cassert>

namespace hp {
void hp::TrainingPhase()
{
std::cout << "Hopfield Network Creation.\n";
  std::srand(static_cast<unsigned>(std::time(nullptr)));
  std::cout << "Insert the number of pixels on the side of the image you want to use (32~128): ";
  unsigned lato{};
  std::cin >> lato;
  assert(32 <= lato && lato <= 128 && "Image size is too low or too high."); 
  hp::Matrix matrix(lato);
  unsigned int imgsGotten = 0;
  while (true) {
    std::cout << "[" << imgsGotten + 1 << "] Insert the name of a png file (\'list\' to see the names) or write \'stop\': ";
    std::string imgName;
    std::cin >> imgName;
    if (imgName == "stop") {
      break;
    }
    hp::Pattern p(lato);
    
    if (imgName == "list") {
      std::cout << " R2D2 \n Vader\n Grogu\n Trooper\n Yoda\n BobaFett\n"; ;
    } else if (hp::loadFromImage(imgName, p)) {
      hp::display(lato, p.getData());
      std::cout << "Got it, teaching the network.\n";
      matrix.learnPattern(p);
      imgsGotten++;
    }
  }
  assert(imgsGotten > 0 && "Err: no images learnt.");
  std::cout << "Training phase completed.\n";

}
}