#include <cassert>
#include "rete_neurale/pattern.hpp"
#include "rete_neurale/recall.hpp"
#include "rete_neurale/matrix.hpp"

int main() {

    std::cout << "Hopfield Network Creation.\n";
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    const unsigned lato = 50;
    Matrix matrix(lato);
    unsigned int imgsGotten = 0;
    while (true) {
        std::cout << "[" << imgsGotten + 1 << "] Insert the name of a jpg file or write \'stop\': ";
        std::string imgName;
        std::cin >> imgName;
        if (imgName == "stop") { break; }
        Pattern p(lato);
        if (p.loadFromImage(imgName)) { 
            p.display();
            std::cout << "Got it, teaching the network.\n"; // ha senso?
            matrix.learnPattern(p);
            imgsGotten++;
        }
    }
    assert(imgsGotten > 0 && "Err: no images learnt.");
    std::cout << "Training phase completed.\nInsert the name of image you want to corrupt: ";
    std::string testImgName;
    std::cin >> testImgName;
    Pattern p(lato);
    if (p.loadFromImage(testImgName)) {
        float noiseLevel = 0.1f;
        std::cout << "Got it, adding " << (noiseLevel * 100) <<"% of noise.\n";
        p.addNoise(noiseLevel);
        p.display();
        std::cout << "Starting the recall.\n";
        matrix.recall(p);
        std::cout << "Image rebuilt!\n";
        p.display();
    } else {
        std::cout << "Err: couldn't see the image.";
    }
    return 0;
}