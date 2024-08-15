#pragma once



#include <SFML/Graphics.hpp>
#include "neuron.hpp"
#include "link.hpp"


#define NEURON_RADIUS 10
#define NEURON_SPACING 70
#define NEURON_WIDTH 750
#define NEURON_HEIGHT 300

#define INPUT_COUNT 6
#define OUTPUT_COUNT 3
#define MAX_HIDDEN_LAYERS 8
#define MAX_HIDDEN_NODES 10
#define MIN_LINKS INPUT_COUNT + OUTPUT_COUNT


class Genome {
private:
    int id;
    std::vector<Neuron> neurons;
    std::vector<Link> links;

public:
    Genome();
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
    void add_links(int index);
    void add_link();

    void mutate();

    void draw_neurons(std::vector<sf::CircleShape> *circles);
    void draw_links(std::vector<sf::VertexArray> *line);

};