#ifndef ACQUISITION_HPP
#define ACQUISITION_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

sf::Image ridimensionaImmagine(const sf::Image& originale);
std::vector<int> caricaImmaginePerRete(const std::string& nomeFile);
void vettoreAImmagine(const std::vector<int>& rete_input);

#endif