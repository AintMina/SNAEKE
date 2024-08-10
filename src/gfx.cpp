#include "gfx.hpp"
#include "colors.hpp"
#include "random.hpp"


// Function to create a grid of rectangles (cells)
std::vector<sf::RectangleShape> createGrid(int rows, int cols, float cellSize) {
    std::vector<sf::RectangleShape> cells;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
            cell.setPosition(j * cellSize, i * cellSize);
            cell.setFillColor(Colors::background); // Default color
            cell.setOutlineColor(Colors::grid);
            cell.setOutlineThickness(1.0f);
            cells.push_back(cell);
        }
    }
    return cells;
}


void drawParameter(std::vector<sf::RectangleShape> *grid, int rows, int cols) {
    
    // TOP and BOTTOM
    for (int i = 0; i < cols; i++) {
        (*grid)[i].setFillColor(Colors::border);
        (*grid)[i + ((rows-1) * cols)].setFillColor(Colors::border);
    }

    // SIDES
    for (int i = 0; i < rows; i++) {
        (*grid)[(i * cols)].setFillColor(Colors::border);
        (*grid)[(i * cols) + cols - 1].setFillColor(Colors::border);
    }
}


void randomPoint(std::vector<sf::RectangleShape> *grid) {
    int randomX = food_rng(generator);
    int randomY = food_rng(generator);
    (*grid)[(randomX) + (randomY * ROWS)].setFillColor(Colors::food);
}