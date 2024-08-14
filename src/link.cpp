#include "link.hpp"
#include "neuron.hpp"
#include "genome.hpp"


Link::Link(int id, int in, int out, double weigth) {
    this->id = id;
    this->in_id = in;
    this->out_id = out;
    this->weigth = weigth;
    this->enabled = true;
    this->calculated = 0;
}

// Enables the link
void Link::enable() {
    this->enabled = true;
}

// Disables the link
void Link::disable() {
    this->enabled = false;
}

int Link::get_id() const {
    return this->id;
}

void Link::set_id(int id) {
    this->id = id;
}

// Returns the value of the link
double Link::get_weigth() {
    return this->weigth;
}

// Returns the value of the link
void Link::set_weigth(double weight) {
    this->weigth = weight;
}

// Returns the value of the link
int Link::get_in() {
    return this->in_id;
}

// Returns the value of the link
void Link::set_in(int in) {
    this->in_id = in;
}

// Returns the value of the link
int Link::get_out() {
    return this->out_id;
}

// Returns the value of the link
void Link::set_out(int out) {
    this->out_id = out;
}

// Calculates the value of the link by multiplying the input value by the weight
void Link::calculate_value(std::vector<Link>& links, std::vector<Neuron>& neurons) {
    if (this->enabled) {
        for (int i = 0; i < neurons.size(); i++) {
            if (this->in_id == neurons[i].get_id()) {
                this->value = neurons[i].get_value(links, neurons) * this->weigth;
                break;
            }
        }
    } else {
        this->value = 0.0;
    }
    this->calculated = true;
}

// Returns the value of the link
double Link::get_value(std::vector<Link>& links, std::vector<Neuron>& neurons) {
    if (this->calculated) {
        return this->value;
    }
    else {
        this->calculate_value(links, neurons);
    }

    return this->value;
}
