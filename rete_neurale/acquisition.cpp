#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

const unsigned int lato = 50;

sf::Image ridimensionaImmagine(const sf::Image& originale) {
    if (originale.getSize().x == lato && originale.getSize().y == lato) {
        return originale;
    } //controllo se va già bene

    // creo una tela (RenderTexture) delle dimensioni che vogliamo
    sf::RenderTexture renderTexture;

    if (!renderTexture.create(lato, lato)) {
        std::cerr << "Errore nel ridimensionamento dell'immagine." << std::endl;
        return originale; // fallback
    }

    // creo uno sprite con l'immagine originale
    sf::Texture texture;
    texture.loadFromImage(originale);
    sf::Sprite sprite(texture);

    // calcolo quanto stirare l'immagine
    float scalaX = static_cast<float>(lato) / originale.getSize().x;
    float scalaY = static_cast<float>(lato) / originale.getSize().y;
    sprite.setScale(scalaX, scalaY);

    // Disegniamo lo sprite ridimensionato sulla tela
    renderTexture.clear(); // pulisce
    renderTexture.draw(sprite); // disegna
    renderTexture.display(); // finalizza

    // Estraiamo la nuova immagine dalla tela
    return renderTexture.getTexture().copyToImage();
}

// trasforma il vettore in un'immagine visibile
void vettoreAImmagine(const std::vector<int>& rete_input) {
    sf::Image immagineVisiva;
    immagineVisiva.create(lato, lato); // Crea una tela vuota nera

    for (unsigned int y = 0; y < lato; y++) {
        for (unsigned int x = 0; x < lato; x++) {
            int indice = y * lato + x;
            int valore = rete_input[indice];

            // Se nella rete è 1, lo coloriamo di BIANCO, se -1 di NERO
            if (valore == 1) {
                immagineVisiva.setPixel(x, y, sf::Color::White);
            } else {
                immagineVisiva.setPixel(x, y, sf::Color::Black);
            }
        }
    }
    sf::Texture texture;
    texture.loadFromImage(immagineVisiva);
    sf::Sprite sprite(texture);
    sprite.setScale(10.0f, 10.0f); // Ingrandisce 10 volte
    sf::RenderWindow window(sf::VideoMode(500, 500), "Rete Neurale - Visualizzatore");

    while (window.isOpen()) {
        sf::Event event;
        // Controlla se hai cliccato la X per chiudere
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // DISEGNA
        window.clear();      // 1. Pulisci tutto (schermo nero)
        window.draw(sprite); // 2. Disegna l'immagine
        window.display();    // 3. Mostra il risultato
    }

}

// Funzione Principale: Carica, Ridimensiona e Converte
std::vector<int> caricaImmaginePerRete(std::string& nomeFile) {
    sf::Image immagineGrezza;
    nomeFile = "/home/amasini/masi/progetto/progetto/images/" + nomeFile;
    if (!immagineGrezza.loadFromFile(nomeFile)) {
        std::cerr << "Errore: Impossibile caricare il file " << nomeFile << std::endl;
        return {}; 
    }

    //ridimensiono
    sf::Image immagine = ridimensionaImmagine(immagineGrezza);

    unsigned int width = immagine.getSize().x;
    unsigned int height = immagine.getSize().y;

    std::vector<int> input_rete;
    input_rete.reserve(width * height);

    // conversione in -1 / +1
    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            sf::Color colore = immagine.getPixel(x, y);
            int luminosita = (static_cast<int>(colore.r) + static_cast<int>(colore.g) + static_cast<int>(colore.b)) / 3;

            if (luminosita > 127) input_rete.push_back(1); 
            else input_rete.push_back(-1);
        }
    }

    vettoreAImmagine(input_rete);

    return input_rete;
}