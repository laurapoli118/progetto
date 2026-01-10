#include "rete_neurale/acquisition.hpp"
#include "rete_neurale/pattern.hpp"
#include "rete_neurale/recall.hpp"
#include "rete_neurale/train.hpp"

int main() {

    char y{};
    do {
        std::cout << "Inserisci il nome di un file jpg: ";
        std::string imgName;
        std::cin >> imgName;
        caricaImmaginePerRete(imgName);
        std::cout << "Vuoi caricare altre foto? (y/n)\n";
        std::cin >> y;
    } while (y == 'y');

    return 0;
}