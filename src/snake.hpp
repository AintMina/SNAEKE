#pragma once


#include <cstdint>
#include <SFML/Graphics.hpp>
#include <vector>


struct Vision {
    uint8_t forward;
    uint8_t left;
    uint8_t right;
};

class Snake {

private:
    /* data */
    sf::RectangleShape head;
    uint8_t head_coords[2] = {10, 10};
    int dir = 0;
    std::vector<sf::RectangleShape> body;
    uint16_t length = 3;
    float cell_size;
    uint8_t grow_flag = 0;
    uint8_t speed = 1;
    uint8_t turning_flag = 0;
    uint8_t alive = 1;
    sf::Color color;
    int food[2] = {0, 0};
    int age = 0;
    int id = 0;

public:
    struct Vision good;
    struct Vision bad;

    Snake();
    Snake(int id);
    Snake(float x, float y, sf::Color color, int dir);

    void reset();
    void reset(float x, float y, int dir);
    int move_snake(std::vector<sf::RectangleShape> *grid);
    void draw_snake(sf::RenderWindow *window);
    void draw_food(sf::RenderWindow *window);
    void turn(float x, float y);
    void turn(float x);
    void grow();
    sf::Color get_next_cell(std::vector<sf::RectangleShape> *grid, int offset[2]);
    void look(std::vector<sf::RectangleShape> *grid);
    uint8_t is_alive();
    void generate_food();
    int get_age();
    int get_length();

};
