#include "userInterface.hpp"
#include "acquisition.hpp"
#include "matrix.hpp"
#include "pattern.hpp"
#include <cassert>

namespace hp {

void TrainingPhase(Matrix& matrix, unsigned side)
{
  unsigned int imgsGotten = 0;
  while (true) {
    std::cout << "[" << imgsGotten + 1
              << "] Insert the name of a png file (\'list\' to see the names) "
                 "or write \'stop\': ";
    std::string imgName;
    std::cin >> imgName;
    if (imgName == "stop") {
      break;
    }
    Pattern p(side);
    if (imgName == "list") {
      std::cout << "R2D2\nVader\nGrogu2\nTrooper\nYoda\nBobaFett\n";
    } else if (loadFromImage(imgName, p)) {
      if (matrix.learnPattern(p)) {
        display(p);
        std::cout << "Got it, teaching the network.\n";
        imgsGotten++;
      }
    }
  }
  assert(imgsGotten > 0 && "Err: no images learnt.");
  std::cout << "Training phase completed.\n";
}

void RecallPhase(Matrix& matrix, unsigned side)
{
  while (true) {
    std::cout
        << "Insert the name of image you want to corrupt or write \'stop\' : ";
    std::string testImgName;
    std::cin >> testImgName;
    if (testImgName == "stop") {
      break;
    }
    hp::Pattern current(side);
    hp::Pattern dirty(side);
    if (loadFromImage(testImgName, current)) {
      std::cout << "Insert the percentage you want to corrupt the image with: ";
      float noiseLevel;
      std::cin >> noiseLevel;
      assert(noiseLevel >= 0 && noiseLevel <= 100
             && "Err: NoiseLevel must be between 0 and 100.");

      std::cout << "Got it, adding " << (noiseLevel) << "% of noise.\n";
      dirty = current;
      dirty.addNoise(noiseLevel / 100);
      display(dirty);
      std::cout << "Starting the recall.\n";
      std::vector<float> energy = matrix.recall(dirty);

      if (dirty.isIdentical(current)) { // check conv è un bool
        std::cout << "The network successfully recalled the image!\n";
      } else {
        std::cout << "The network couldn't fully recall the image.\n";
      }

      std::cout << "Initial energy: " << energy.front()
                << "\nFinal energy: " << energy.back()
                << "\nTotal steps: " << energy.size() << '\n';

      display(dirty);
    } else {
      std::cout << "Err: couldn't see the image.";
    }
  }
}

} // namespace hp