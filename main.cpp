#include "rete_neurale/acquisition.hpp"
#include "rete_neurale/matrix.hpp"
#include "rete_neurale/pattern.hpp"
#include <cassert>

int main()
{
  std::cout << "Hopfield Network Creation.\n";
  std::srand(static_cast<unsigned>(std::time(nullptr)));
  std::cout << "Insert the number of pixels you want to use (32~128): ";
  unsigned lato{};
  std::cin >> lato;
  assert(32 <= lato && lato <= 128 && "Image size is too low or too high."); // REGA QUI GEMINI CONSIGLIA ANCORA DI NON USARE ASSERT PER GLI USER MISTAKES
  Matrix matrix(lato);
  unsigned int imgsGotten = 0;
  while (true) {
    std::cout << "[" << imgsGotten + 1 << "] Insert the name of a png file (Mario, Luigi, Toad, Bowser or \'all\') or write \'stop\': ";
    std::string imgName;
    std::cin >> imgName;
    if (imgName == "stop") {
      break;
    }
    Pattern p(lato);
    if (imgName == "all") {
      std::string images[] = {"Mario", "Luigi", "Bowser", "Toad"};
      for (int i = 0; i < 4; i++) {
        Acquisition::loadFromImage(images[i], p);
        matrix.learnPattern(p);
        imgsGotten++;
      }
      break;
    }
    if (Acquisition::loadFromImage(imgName, p)) {
      Acquisition::display(lato, p.getData());
      std::cout << "Got it, teaching the network.\n";
      matrix.learnPattern(p);
      imgsGotten++;
    }
  }
  assert(imgsGotten > 0 && "Err: no images learnt.");
  std::cout << "Training phase completed.\n";
  while (true) {
    std::cout
        << "Insert the name of image you want to corrupt or write \'stop\' : ";
    std::string testImgName;
    std::cin >> testImgName;
    if (testImgName == "stop") {
      break;
    }
    Pattern current(lato);
    Pattern dirty(lato);
    if (Acquisition::loadFromImage(testImgName, current)) {
      std::cout << "Insert the percentage you want to corrupt the image with: ";
      float noiseLevel;
      std::cin >> noiseLevel;
      assert(noiseLevel >= 0 && noiseLevel <= 100
             && "Err: NoiseLevel must be between 0 and 100."); // STESSA COSA DI SOPRA PER ASSERT (throw?)
      

      std::cout << "Got it, adding " << (noiseLevel) << "% of noise.\n";
      dirty = current; // invece che rifargli caricare la funzione glielo copio da current
      dirty.addNoise(noiseLevel / 100);
      Acquisition::display(lato, dirty.getData());
      std::cout << "Starting the recall.\n";
      std::vector<float> energy = matrix.recall(dirty);

      if (dirty.isIdentical(current)) {     //check conv è un bool
        std::cout << "The network successfully recalled the image!\n";
      } else {
        std::cout << "The network couldn't fully recall the image.\n";
      }

      std::cout << "Initial energy: " << energy.front() << "\nFinal energy: " << energy.back() << "\nTotal steps: " << energy.size() << '\n';

      Acquisition::display(lato, dirty.getData());
    } else {
      std::cout << "Err: couldn't see the image.";
    }
  }
  return 0;
}