#include "genome.hpp"
#include "random.hpp"
#include "colors.hpp"
#include <algorithm>
#include <iostream>



Genome::Genome() {
}

Genome::Genome(int id) {
    this->id = id;

    for (int i = 0; i < INPUT_COUNT; i++) {
        Neuron neuron(i, 0.0);
        neuron.set_value(0.0);
        this->neurons.push_back(neuron);
    }
    for (int i = 0; i < OUTPUT_COUNT; i++) {
        Neuron neuron((MAX_HIDDEN_LAYERS * MAX_HIDDEN_NODES) + i, 0.0);
        neuron.set_value(0.0);
        this->neurons.push_back(neuron);
    }
}

Genome::Genome(int id, const std::vector<Neuron> neurons) {
    this->id = id;

    for (int i = 0; i < INPUT_COUNT+OUTPUT_COUNT; i++) {
        this->neurons.push_back(neurons[i]);
    }
}

Genome::Genome(int id, const std::vector<Neuron> neurons, const std::vector<Link> links) {
    this->id = id;

    for (int i = 0; i < neurons.size(); i++) {
        this->neurons.push_back(neurons[i]);
    }
    for (int i = 0; i < links.size(); i++) {
        this->links.push_back(links[i]);
    }
}


void Genome::clear() {
    this->neurons.clear();
    this->links.clear();

    for (int i = 0; i < INPUT_COUNT; i++) {
        Neuron neuron(i, 0.0);
        neuron.set_value(0.0);
        this->neurons.push_back(neuron);
    }
    for (int i = 0; i < OUTPUT_COUNT; i++) {
        Neuron neuron((MAX_HIDDEN_LAYERS * MAX_HIDDEN_NODES) + i, 0.0);
        neuron.set_value(0.0);
        this->neurons.push_back(neuron);
    }
}


void Genome::calculated() {
    for (int i = 0; i < this->neurons.size(); i++) {
        this->neurons[i].calculated = false;
    }
    for (int i = 0; i < this->links.size(); i++) {
        this->links[i].calculated = false;
    }
}

void Genome::populate() {
    double random = get_neural_rng();
    random *= 4;
    random++;
    
    // Generate neurons
    for (int layer = 1; layer < random; layer++) {
        double random_layer = get_neural_rng() * (MAX_HIDDEN_LAYERS-2);
        double random_nodes = get_neural_rng() * 3;

        for (int node = 0; node < random_nodes; node++) {
            Neuron neuron(((random_layer+1)*MAX_HIDDEN_NODES), get_neural_rng());
            double random_bias = get_neural_rng() * 2.0;
            random_bias -= 1.0;
            neuron.set_bias(random_bias);
            this->add_neuron(neuron);
        }
    }

    this->neurons.push_back(this->neurons[INPUT_COUNT]);
    this->neurons.push_back(this->neurons[INPUT_COUNT+1]);
    this->neurons.push_back(this->neurons[INPUT_COUNT+2]);
    // Generate links
    for (int i = 0; i < this->neurons.size(); i++) {
        if (this->neurons[i].get_layer() >= MAX_HIDDEN_LAYERS) {
            continue;
        }

        random = get_neural_rng();
        random += 1.0;
        for (int ii = 0; ii < random; ii++) {
            int in = this->neurons[i].get_id();
            double random_neuron = get_neural_rng();
            random_neuron *= (this->neurons.size() - i);

            while (this->neurons[i + random_neuron].get_layer() <= this->neurons[i].get_layer()) {
                random_neuron += 1.0;
            }

            if (i + random_neuron >= this->neurons.size()) {
                ii--;
                continue;
            }

            if (this->neurons[i + random_neuron].get_layer() <= this->neurons[i].get_layer()) {
                continue;
            }

            int out_index = 0;
            if (i + random_neuron >= this->neurons.size() - 3) {
                out_index = (i + random_neuron) - (this->neurons.size() - 3);
                out_index += INPUT_COUNT;
            }
            else {
                out_index = i + random_neuron;
            }

            int out = this->neurons[out_index].get_id();

            int id = in * (MAX_HIDDEN_LAYERS * MAX_HIDDEN_NODES);
            id += out;

            int same_link = 0;
            for (int iii = 0; iii < this->links.size(); iii++) {
                if (this->links[iii].get_id() == id) {
                    ii--;
                    same_link = 1;
                    break;
                }
            }

            if (same_link) {
                continue;
            }

            this->neurons[i].add_link_out(id);
            this->neurons[out_index].add_link_in(id);

            random_neuron = get_neural_rng() * 2;
            random_neuron -= 1.0;
            Link link(in, out, random_neuron);

            this->links.push_back(link);
        }
    }
    this->neurons.pop_back();
    this->neurons.pop_back();
    this->neurons.pop_back();
}

// Add a neuron to the genome
void Genome::add_neuron(Neuron neuron) {
    int ii = 0;
    for (int i = 0; i < this->neurons.size(); i++) {
        if (neuron.get_layer() == this->neurons[i].get_layer()) {
            ii++;
        }
    }

    neuron.set_id((neuron.get_layer() * MAX_HIDDEN_NODES) + ii);

    neurons.push_back(neuron);
}

// Get the genome's ID
int Genome::get_id() const {
    return this->id;
}

// Set the input values for the input neurons
void Genome::set_input(const double* inputs) {
    for (int i = 0; i < INPUT_COUNT; ++i) {
        if (i < neurons.size()) {
            this->neurons[i].set_value(inputs[i]);
            this->neurons[i].calculated = true;
        }
    }
}

void Genome::update_network() {
    for (int i = INPUT_COUNT+OUTPUT_COUNT-1; i < this->neurons.size(); i++) {
        this->neurons[i].get_value(this->links, this->neurons);
    }
}

// Get the output values
void Genome::get_output(double *buffer) {
    for (int i = 0; i < OUTPUT_COUNT; i++) {
        buffer[i] = this->neurons[i + INPUT_COUNT].get_value(this->links, this->neurons);
    }
}

void Genome::delete_neuron() {
    if (this->neurons.size() <= (INPUT_COUNT + OUTPUT_COUNT)) {
        return;
    }

    int random = (INPUT_COUNT + OUTPUT_COUNT) + (get_neural_rng() * (this->neurons.size() - INPUT_COUNT - OUTPUT_COUNT));

    while (random > this->neurons.size()) {
        random--;
    }

    std::vector<int> in = this->neurons[random].get_in();
    std::vector<int> out = this->neurons[random].get_out();

    for (int i = 0; i < this->links.size(); i++) {
        for (int ii = 0; (ii < in.size() || ii < out.size()); ii++) {
            if (ii < in.size() && this->links[i].get_id() == in[ii]) {
                std::vector<Link>::iterator it = this->links.begin();
                std::advance(it, i);
                this->links.erase((it));
                i--;
            }
            if (ii < out.size() && this->links[i].get_id() == out[ii]) {
                std::vector<Link>::iterator it = this->links.begin();
                std::advance(it, i);
                this->links.erase((it));
                i--;
            }
        }
    }
    std::vector<Neuron>::iterator it = this->neurons.begin();
    std::advance(it, random);
    this->neurons.erase(it);
}

void Genome::delete_link() {
    if (this->links.size() < MIN_LINKS) {
        return;
    }

    int random = get_neural_rng() * this->links.size();

    for (int i = 0; i < this->neurons.size(); i++) {
        std::vector<int> in = this->neurons[i].get_in();
        std::vector<int> out = this->neurons[i].get_out();
        
        for (int ii = 0; (ii < in.size() || ii < out.size()); ii++) {
            if (ii < in.size() && in[ii] == this->links[random].get_id()) {
                this->neurons[i].remove_in(ii);
                std::vector<int>::iterator it = in.begin();
                std::advance(it, ii);
                in.erase(it);
                ii--;
            }
            else if (ii < out.size() && out[ii] == this->links[random].get_id()) {
                this->neurons[i].remove_out(ii);
                std::vector<int>::iterator it = out.begin();
                std::advance(it, ii);
                out.erase(it);
                ii--;
            }
        }
    }

    std::vector<Link>::iterator it = this->links.begin();
    std::advance(it, random);
    this->links.erase(it);

}

void Genome::modify_weight() {
    int random = get_neural_rng() * this->links.size();
    double random_value = (get_food_rng() * 2) - 1.0;
    this->links[random].set_weigth(random_value);
}

void Genome::modify_bias() {
    int random = 0;
    random = get_neural_rng() * this->neurons.size();

    while (random >= INPUT_COUNT && random < (INPUT_COUNT + OUTPUT_COUNT)) {
        random = get_neural_rng() * this->neurons.size();
    }

    double random_value = (get_food_rng() * 2) - 1.0;
    this->neurons[random].set_bias(random_value);
}

void Genome::add_neuron() {
    for (int trying = 0; trying < 5; trying++) {
        int random_column = (get_neural_rng() * (MAX_HIDDEN_LAYERS - 2) + 1);

        int items = 0;
        for (int i = 0; i < this->neurons.size(); i++) {
            if (this->neurons[i].get_layer() == random_column) {
                items++;
            }
        }

        if (items >= MAX_HIDDEN_NODES) {
            continue;
        }

        double random_value = (get_neural_rng() * 2) - 1.0;
        Neuron neuron((random_column * MAX_HIDDEN_NODES) + items, random_value);
        this->neurons.push_back(neuron);
        this->add_links(this->neurons.size()-1);
        break;
    }
}

void Genome::add_links(int index) {
    int layer = this->neurons[index].get_layer();
    std::vector<int> items;
    std::vector<int> items_out;

    // Input link
    for (int i = 0; i < this->neurons.size(); i++) {
        if (this->neurons[i].get_layer() < layer) {
            items.push_back(i);
        }
        else if (this->neurons[i].get_layer() > layer) {
            items_out.push_back(i);
        }
    }

    if (items_out.size() == 0) {
        return;
    }

    int random = get_neural_rng() * items.size()-1;
    int in_id = this->neurons[items[random]].get_id();
    int out_id = this->neurons[index].get_id();
    double random_value = (get_neural_rng() * 2) - 1.0;

    Link link(in_id, out_id, random_value);
    this->neurons[index].add_link_in(link.get_id());
    this->neurons[items[random]].add_link_out(link.get_id());
    this->links.push_back(link);

    // Output link
    random = get_neural_rng() * items_out.size();
    random = std::clamp(random, 0, static_cast<int>(items_out.size()-1));
    int out_item = std::clamp(items_out[random], 0, static_cast<int>(neurons.size()-1));
    in_id = this->neurons[index].get_id();
    out_id = this->neurons[out_item].get_id();
    random_value = (get_neural_rng() * 2) - 1.0;

    Link link_out(in_id, out_id, random_value);
    this->neurons[items_out[random]].add_link_in(link_out.get_id());
    this->neurons[index].add_link_out(link_out.get_id());
    this->links.push_back(link_out);
}

void Genome::add_link() {
    int layer_in = 255;
    int in = 0;
    int layer_out = 0;
    int out = 0;

    for (int i = 0; i < 5; i++) {
        int random = get_neural_rng() * this->neurons.size()-1;

        if (this->neurons[random].get_layer() < layer_in) {
            layer_in = this->neurons[random].get_layer();
            in = random;
        }
        if (this->neurons[random].get_layer() > layer_out) {
            layer_out = this->neurons[random].get_layer();
            out = random;
        }

    }

    if (layer_in >= layer_out) {
        return;
    }

    int in_id = this->neurons[in].get_id();
    int out_id = this->neurons[out].get_id();
    double random_value = (get_neural_rng() * 2) - 1.0;
    Link link(in_id, out_id, random_value);
    this->neurons[out].add_link_in(link.get_id());
    this->neurons[in].add_link_out(link.get_id());
    this->links.push_back(link);

}

void Genome::mutate() {
    int random = (get_neural_rng()) + 1;
    for (int i = 0; i < random; i++) {
        int random_effect = (get_neural_rng() * 6);

        if (this->neurons.size() > 20) {
            random_effect /= 2;
        }
        else if (this->neurons.size() <= OUTPUT_COUNT + INPUT_COUNT + 1) {
            random_effect = 4;
        }

        switch (random_effect) {
            case 0:
                this->delete_neuron();
                break;
            case 1:
                this->delete_link();
                break;
            case 2:
                this->modify_weight();
                break;
            case 3:
                this->modify_bias();
                break;
            case 4:
                this->add_neuron();
                break;
            case 5:
                this->add_link();
                break;

            default:
                break;
        }
    }
}

void Genome::draw_neurons(std::vector<sf::CircleShape> *circles) {
    for (int neuron = 0; neuron < this->neurons.size(); neuron++) {
        sf::CircleShape circle(NEURON_RADIUS);
        double value = (this->neurons[neuron].value + 1) / 2;
        if (value <= 1) {
            value *= 255;
        }
        int color = value;
        circle.setFillColor(sf::Color(255-color, color, 0, 255));
        circle.setOutlineColor(Colors::node_out_color);
        circle.setOutlineThickness(3);

        int width_spacing = NEURON_WIDTH;
        width_spacing /= MAX_HIDDEN_LAYERS+2;

        int index = 0;
        int count = 0;
        int column = this->neurons[neuron].get_layer();

        for (int i = 0; i < this->neurons.size(); i++) {
            if (this->neurons[i].get_layer() == column) {
                if (i < neuron) {
                    index++;
                }
                count++;
            }
        }

        int height_spacing = NEURON_HEIGHT;
        if (count) {
            height_spacing /= (count+1);
        }
        else {
            height_spacing = (NEURON_HEIGHT/2);
        }

        this->neurons[neuron].position[0] = ((column+1) * width_spacing) - NEURON_RADIUS;
        this->neurons[neuron].position[1] = ((index+1) * height_spacing) - NEURON_RADIUS;
        circle.setPosition(this->neurons[neuron].position[0], this->neurons[neuron].position[1]);
        circles->push_back(circle);

    }
}

void Genome::draw_links(std::vector<sf::VertexArray> *line) {
    for (int i = 0; i < this->links.size(); i++) {
        int start_x = 0;
        int start_y = 0;

        for (int ii = 0; ii < this->neurons.size(); ii++) {
            if (this->neurons[ii].get_id() == this->links[i].get_in()) {
                start_x = this->neurons[ii].position[0];
                start_y = this->neurons[ii].position[1];
                break;
            }
        }

        int end_x = 0;
        int end_y = 0;

        for (int ii = 0; ii < this->neurons.size(); ii++) {
            if (this->neurons[ii].get_id() == this->links[i].get_out()) {
                end_x = this->neurons[ii].position[0];
                end_y = this->neurons[ii].position[1];
                break;
            }
        }

        sf::Vector2f start(start_x + NEURON_RADIUS, start_y + NEURON_RADIUS);
        sf::Vector2f end(end_x + NEURON_RADIUS, end_y + NEURON_RADIUS);
        sf::VertexArray new_line(sf::Lines, 2);

        new_line[0].position = start;
        new_line[1].position = end;

        double value = (this->links[i].value + 1.0) / 2;
        int color = value * 255;
        new_line[0].color = sf::Color(255-color, color, 0, 255);
        new_line[1].color = sf::Color(255-color, color, 0, 255);

        line->push_back(new_line);

    }
}