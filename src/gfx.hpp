#pragma once



#include <SFML/Graphics.hpp>
#include <vector>

// Function to create a grid of rectangles (cells)
std::vector<sf::RectangleShape> createGrid(int rows, int cols, float cellSize);
void drawParameter(std::vector<sf::RectangleShape> *grid, int rows, int cols);
void randomPoint(std::vector<sf::RectangleShape> *grid);
