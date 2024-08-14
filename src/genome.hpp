#pragma once



#include <SFML/Graphics.hpp>
#include "neuron.hpp"


#define NEURON_RADIUS 20
#define NEURON_SPACING 70
#define NEURON_WIDTH 1500
#define NEURON_HEIGHT 600

#define INPUT_COUNT 6
#define OUTPUT_COUNT 3
#define MAX_HIDDEN_LAYERS 8
#define MAX_HIDDEN_NODES 10


class Genome {
private:
    int id;
    std::vector<Neuron> neurons;
    std::vector<Link> links;

public:
    Genome(int id);
    Genome(int id, const std::vector<Neuron> neurons);
    Genome(int id, const std::vector<Neuron> neurons, const std::vector<Link> links);

    void clear();
    void calculated();
    void populate();
    void add_neuron(Neuron neuron);

    int get_id() const;
    void set_input(const double *inputs);
    void update_network();
    void get_output(double *buffer);

    void delete_neuron();
    void delete_link();
    void modify_weight();
    void modify_bias();
    void add_neuron();
    void add_link();

    void draw_neurons(std::vector<sf::CircleShape> *circles);
    void draw_links(std::vector<sf::VertexArray> *line);

};


struct Individuals {
    Genome genome;
    double fitness = 0.0;
};