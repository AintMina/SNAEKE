#pragma once



#include "settings.hpp"
#include "genome.hpp"
#include "snake.hpp"
#include <SFML/Graphics.hpp>


class Specimen {
private:
    Genome gene;
    Snake snake;
    int id;
    double fitness = 0.0;
    int age = 0;

public:
    bool calculated = false;
    
    Specimen(int id);

    void clear();

    int get_id() const;
    void set_id(int id);

    void update(std::vector<sf::RectangleShape> *grid);
    void mutate();
    void calculate_fitness(int max_age);
    double get_fitness(int max_age);
    void draw_specimen(sf::RenderWindow *genome_window);
    void draw_snake(sf::RenderWindow *window);

};