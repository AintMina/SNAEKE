#pragma once



#include <SFML/Graphics.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/xml.hpp>
#include "neuron.hpp"
#include "link.hpp"


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

    template<class Archive>
    void serialize(Archive& archive) {
        archive(id, neurons, links);
    }

};