#pragma once


#include <cstdint>
#include <SFML/Graphics.hpp>
#include <vector>

class Snake {

private:
    /* data */
    sf::RectangleShape head;
    uint8_t head_coords[2];
    int dir = 0;
    std::vector<sf::RectangleShape> body;
    uint16_t length = 3;
    float cell_size;
    uint8_t grow_flag = 0;
    uint8_t speed = 1;
    uint8_t turning_flag = 0;
    uint8_t alive = 1;
    sf::Color color;

public:
    Snake(float x, float y, float cell_size, sf::Color color, int dir);

    void reset(float x, float y, float cell_size, sf::Color color, int dir);
    int move_snake(std::vector<sf::RectangleShape> *grid);
    void draw_snake(sf::RenderWindow *window);
    void turn(float x, float y);
    void grow();
    sf::Color get_next_cell(std::vector<sf::RectangleShape> *grid, int offset[2]);

};
