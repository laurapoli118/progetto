#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

const unsigned int lato = 50;

// Funzione ausiliaria: Ridimensiona un'immagine SFML
sf::Image ridimensionaImmagine(const sf::Image& originale) {
    if (originale.getSize().x == lato && originale.getSize().y == lato) {
        return originale;
    } //controllo se va già bene

    // creo una tela (RenderTexture) delle dimensioni che vogliamo
    sf::RenderTexture renderTexture(lato, lato);

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

// Funzione Principale: Carica, Ridimensiona e Converte
std::vector<int> caricaImmaginePerRete(const std::string& nomeFile) {
    sf::Image immagineGrezza;
    
    // Caricamento
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
    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            sf::Color colore = immagine.getPixel(x, y);
            int luminosita = (static_cast<int>(colore.r) + static_cast<int>(colore.g) + static_cast<int>(colore.b)) / 3;

            if (luminosita > 127) input_rete.push_back(1); 
            else input_rete.push_back(-1);
        }
    }

    return input_rete;
}