#include "rete_neurale/pattern.hpp"
#include "rete_neurale/recall.hpp"
#include "rete_neurale/train.hpp"

int main() {

    char y{};
    do {
        Pattern p(50);
        std::cout << "Inserisci il nome di un file jpg: ";
        std::string imgName;
        std::cin >> imgName;
        if (p.loadFromImage(imgName)) { p.display(); }
        std::cout << "Vuoi caricare altre foto? (y/n)\n";
        std::cin >> y;
    } while (y == 'y');

    return 0;
}