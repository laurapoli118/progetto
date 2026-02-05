#ifndef RECALL_HPP
#define RECALL_HPP
#include "pattern.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

namespace hp {

sf::Image resize(const sf::Image& originale, unsigned size);
bool loadFromImage(const std::string& filename, hp::Pattern& pattern);
void display(unsigned size, const std::vector<int>& data);
} 

#endif