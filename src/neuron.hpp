#pragma once



#include <vector>
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/array.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/xml.hpp>


class Link;


class Neuron {
private:
    int id;
    double bias;
    std::vector<int> in;
    int in_count;
    std::vector<int> out;
    int out_count;
    int layer = 0;

public:
    double value = 0.0;
    std::array<int, 2> position = {0, 0};
    bool calculated = false;
    
    Neuron();
    Neuron(int id, double bias);
    Neuron(int id, const std::vector<int>& in, const std::vector<int>& out, double bias);
    Neuron(int id, double bias, const std::vector<int>& in, int in_count, const std::vector<int>& out, int out_count, const std::array<int, 2>& position, int layer);

    void add_link_in(int in);
    void add_link_out(int out);

    int get_id() const;
    void set_id(int id);

    double get_bias() const;
    void set_bias(double bias);

    std::vector<int> get_in() const;
    void set_in(int in_id);
    void remove_in(int index);
    std::vector<int> get_out() const;
    void set_out(int out_id);
    void remove_out(int index);

    double activation(double in) const;
    void calculate_value(std::vector<Link> links, std::vector<Neuron> neurons);
    void set_value(double value);
    double get_value(std::vector<Link> links, std::vector<Neuron> neurons);
    void set_layer(int layer);
    int get_layer();

    template<class Archive>
    void serialize(Archive& archive) {
        archive(id, bias, in, in_count, out, out_count, position, layer);
    }

};