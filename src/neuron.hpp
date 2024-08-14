#pragma once



#include <vector>


class Link;


class Neuron {
private:
    int id;
    std::vector<int> in;
    std::vector<int> out;
    double bias;
    int layer = 0;

public:
    double value = 0.0;
    int position[2] = {0, 0};
    bool calculated = false;
    
    Neuron(int id, double bias);
    Neuron(int id, const std::vector<int>& in, const std::vector<int>& out, double bias);

    void add_link_in(int in);
    void add_link_out(int out);

    int get_id() const;
    void set_id(int id);

    double get_bias() const;
    void set_bias(double bias);

    std::vector<int> get_in() const;
    void set_in(int in_id);
    std::vector<int> get_out() const;
    void set_out(int out_id);

    double activation(double in) const;
    void calculate_value(std::vector<Link>& links, std::vector<Neuron>& neurons);
    void set_value(double value);
    double get_value(std::vector<Link>& links, std::vector<Neuron>& neurons);
    void set_layer(int layer);
    int get_layer();

};