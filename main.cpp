#include "rete_neurale/acquisition.hpp"
#include "rete_neurale/pattern.hpp"
#include "rete_neurale/recall.hpp"
#include "rete_neurale/train.hpp"

int main() {
    for (int i=0; i<5; i++) {
        std::cout << "Inserisci il nome di un file jpg: ";
        std::string imgName;
        std::cin >> imgName;
        caricaImmaginePerRete(imgName);
        std::cout << '\n';
    }

    return 0;
}