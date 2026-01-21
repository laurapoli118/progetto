#include <cassert>
#include "rete_neurale/pattern.hpp"
#include "rete_neurale/recall.hpp"
#include "rete_neurale/matrix.hpp"

int main() {

    std::cout << "Hopfield Network Creation.\n";
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    std::cout << "Insert the number of pixels you want to use (50-100): ";
    unsigned lato{};
    std::cin >> lato;
    assert(32 <= lato && lato <= 128 && "Image size is too low or too high.");
    Matrix matrix(lato);
    unsigned int imgsGotten = 0;
    while (true) {
        std::cout << "[" << imgsGotten + 1 << "] Insert the name of a png file (Mario, Luigi, Toad, Bowser or \'all\') or write \'stop\': ";
        std::string imgName;
        std::cin >> imgName;
        if (imgName == "stop") { break; }
        Pattern p(lato);
        if (imgName == "all") {
            std::string images[] = {"Mario" , "Luigi", "Bowser", "Toad"};
            for (int i=0; i<4; i++) {
                p.loadFromImage(images[i]);
                matrix.learnPattern(p);
                imgsGotten++;
            }
            break;
        }
        if (p.loadFromImage(imgName)) { 
            p.display();
            std::cout << "Got it, teaching the network.\n";
            matrix.learnPattern(p);
            imgsGotten++;
        }
    }
    assert(imgsGotten > 0 && "Err: no images learnt.");
    std::cout << "Training phase completed.\n";
    while (true) {
        std::cout << "Insert the name of image you want to corrupt or write \'stop\' : ";
        std::string testImgName;
        std::cin >> testImgName;
        if (testImgName == "stop") { break; }
        Pattern p(lato);
        if (p.loadFromImage(testImgName)) {
            std::cout << "Insert the percentage you want to corrupt the image with: ";
            float noiseLevel;
            std::cin >> noiseLevel;
            assert(noiseLevel >= 0 && noiseLevel <= 100 && "Err: NoiseLevel must be between 0 and 100.");
            std::cout << "Got it, adding " << (noiseLevel) <<"\% of noise.\n";
            p.addNoise(noiseLevel/100);
            p.display();
            std::cout << "Starting the recall.\n";
            matrix.recall(p);
            std::cout << "Image rebuilt!\n";
            p.display();
        } else {
            std::cout << "Err: couldn't see the image.";
        }
    }
    return 0;
}