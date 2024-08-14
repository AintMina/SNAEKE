#include "neuron.hpp"
#include "link.hpp"
#include "genome.hpp"
#include <iostream>
#include <cmath>


Neuron::Neuron(int id, double bias) {
    this->id = id;
    this->bias = bias;

    int id_tmp = id;
    int layer = 0;
    while (id_tmp >= MAX_HIDDEN_NODES) {
        id_tmp -= MAX_HIDDEN_NODES;
        layer++;
    }

    this->layer = layer;
}
Neuron::Neuron(int id, const std::vector<int>& in, const std::vector<int>& out, double bias) {
    this->id = id;
    this->bias = bias;
    for (int i = 0; i < in.size(); i++) {
        this->in.push_back(in[i]);
    }
    for (int i = 0; i < out.size(); i++) {
        this->out.push_back(out[i]);
    }

    int id_tmp = id;
    int layer = 0;
    while (id_tmp >= MAX_HIDDEN_NODES) {
        id_tmp -= MAX_HIDDEN_NODES;
        layer++;
    }

    this->layer = layer;
}


void Neuron::add_link_in(int in) {
    this->in.push_back(in);
}

void Neuron::add_link_out(int out) {
    this->out.push_back(out);
}

int Neuron::get_id() const {
    return this->id;
}

void Neuron::set_id(int id) {
    this->id = id;
}

double Neuron::get_bias() const {
    return this->bias;
}

void Neuron::set_bias(double bias) {
    this->bias = bias;
}

std::vector<int> Neuron::get_in() const {
    return this->in;
}

void Neuron::set_in(int in_id) {
    this->in.push_back(in_id);
}

std::vector<int> Neuron::get_out() const {
    return this->out;
}

void Neuron::set_out(int out_id) {
    this->out.push_back(out_id);
}

double Neuron::activation(double in) const {
    return (2.0 / (1.0 + std::exp(-in))) - 1.0;  // Sigmoid function
}

void Neuron::calculate_value(std::vector<Link>& links, std::vector<Neuron>& neurons) {
    if (!this->calculated) {
        double sum = static_cast<double>(this->bias);

        for (int link = 0; link < this->in.size(); link++) {
            for (int i = 0; i < links.size(); i++) {
                if (this->in[link] == links[i].get_id()) {
                    sum += links[i].get_value(links, neurons);
                }
            }
        }

        this->value = this->activation(sum);
        this->calculated = true;
    }
}

void Neuron::set_value(double value) {
    this->value = value;
    this->calculated = true;
}

double Neuron::get_value(std::vector<Link>& links, std::vector<Neuron>& neurons) {
    if (this->calculated) {
        return this->value;
    }
    else {
        this->calculate_value(links, neurons);
    }

    return this->value;
}

void Neuron::set_layer(int layer) {
    this->layer = layer;
}

int Neuron::get_layer() {

    return this->layer;
}

