#pragma once



#include <SFML/Graphics/Color.hpp>
#include <vector>

namespace Colors {
    const sf::Color background = sf::Color::Black;
    const sf::Color grid = sf::Color(20, 20, 20, 255);
    const sf::Color snake_color = sf::Color(255, 255, 0, 200);
    const sf::Color snake2_color = sf::Color(0, 255, 255, 200);
    const sf::Color border = sf::Color(255, 0, 0, 100);
    const sf::Color food = sf::Color(0, 255, 0, 100);
    const sf::Color genome_background = sf::Color(20, 20, 20, 255);
    const sf::Color node_in_color = sf::Color(180, 180, 180, 255);
    const sf::Color node_out_color = sf::Color(100, 100, 100, 255);
    const sf::Color link_color = sf::Color(0, 255, 0, 255);
}