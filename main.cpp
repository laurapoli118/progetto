#include "rete_neurale/acquisition.hpp"
#include "rete_neurale/matrix.hpp"
#include "rete_neurale/pattern.hpp"
#include <cassert>

int main()
{
  
  while (true) {
    std::cout
        << "Insert the name of image you want to corrupt or write \'stop\' : ";
    std::string testImgName;
    std::cin >> testImgName;
    if (testImgName == "stop") {
      break;
    }
    hp::Pattern current(lato);
    hp::Pattern dirty(lato);
    if (hp::loadFromImage(testImgName, current)) {
      std::cout << "Insert the percentage you want to corrupt the image with: ";
      float noiseLevel;
      std::cin >> noiseLevel;
      assert(noiseLevel >= 0 && noiseLevel <= 100
             && "Err: NoiseLevel must be between 0 and 100."); 
      

      std::cout << "Got it, adding " << (noiseLevel) << "% of noise.\n";
      dirty = current; 
      dirty.addNoise(noiseLevel / 100);
      hp::display(lato, dirty.getData());
      std::cout << "Starting the recall.\n";
      std::vector<float> energy = matrix.recall(dirty);

      if (dirty.isIdentical(current)) {     //check conv è un bool
        std::cout << "The network successfully recalled the image!\n";
      } else {
        std::cout << "The network couldn't fully recall the image.\n";
      }

      std::cout << "Initial energy: " << energy.front() << "\nFinal energy: " << energy.back() << "\nTotal steps: " << energy.size() << '\n';

      hp::display(lato, dirty.getData());
    } else {
      std::cout << "Err: couldn't see the image.";
    }
  }
  return 0;
}