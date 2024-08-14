#include "genome.hpp"
#include "link.hpp"
#include "random.hpp"
#include "colors.hpp"
#include <algorithm>



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
        if (this->neurons[i].get_layer() == MAX_HIDDEN_LAYERS) {
            continue;
        }

        random = get_neural_rng();
        // random += 1.0;
        for (int ii = 0; ii < random; ii++) {
            int in = this->neurons[i].get_id();
            double random_neuron = get_neural_rng() * ((this->neurons.size() - i) + 3);

            while (this->neurons[i + random_neuron].get_layer() <= this->neurons[i].get_layer()) {
                random_neuron += 1.0;
            }

            if (i + random_neuron >= this->neurons.size()) {
                ii--;
                continue;
            }

            int out = this->neurons[i + random_neuron].get_id();

            int id = in * (MAX_HIDDEN_LAYERS * MAX_HIDDEN_NODES);
            id += out;

            this->neurons[i].add_link_out(id);
            this->neurons[i + random_neuron].add_link_in(id);

            random_neuron = get_neural_rng() * 2;
            random_neuron -= 1.0;
            Link link(id, in, out, random_neuron);

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
    int random = (INPUT_COUNT + OUTPUT_COUNT) + (get_neural_rng() * (this->neurons.size() - INPUT_COUNT - OUTPUT_COUNT));

    while (random > this->neurons.size()) {
        random--;
    }

    std::vector<int> in = this->neurons[random].get_in();
    std::vector<int> out = this->neurons[random].get_out();

    for (int i = 0; i < this->links.size(); i++) {
        for (int ii = 0; ii < in.size(); ii++) {
            if (this->links[i].get_id() == in[ii]) {
                std::vector<Link>::iterator it = this->links.begin();
                std::advance(it, i);
                this->links.erase((it));
            }
            else if (this->links[i].get_id() == out[ii]) {
                std::vector<Link>::iterator it = this->links.begin();
                std::advance(it, i);
                this->links.erase((it));
            }
        }
    }
    std::vector<Neuron>::iterator it = this->neurons.begin();
    std::advance(it, random);
    this->neurons.erase(it);
}

void Genome::delete_link() {

}

void Genome::modify_weight() {

}

void Genome::modify_bias() {

}

void Genome::add_neuron() {

}

void Genome::add_link() {

}

void Genome::draw_neurons(std::vector<sf::CircleShape> *circles) {
    for (int neuron = 0; neuron < this->neurons.size(); neuron++) {
        sf::CircleShape circle(NEURON_RADIUS);
        double value = (this->neurons[neuron].value + 1) / 2;
        int color = value * 255;
        circle.setFillColor(sf::Color(255-color, color, 0, 255));
        circle.setOutlineColor(Colors::node_out_color);
        circle.setOutlineThickness(3);

        int width_spacing = NEURON_WIDTH;
        width_spacing /= MAX_HIDDEN_LAYERS+2;

        int index = 0;
        int count = 0;
        int column = this->neurons[neuron].get_layer();

        for (int i = 0; i < this->neurons.size(); i++) {
            if (this->neurons[i].get_layer() == this->neurons[neuron].get_layer()) {
                if (i < neuron) {
                    index++;
                }
                count++;
            }
            else if (this->neurons[i].get_layer() > this->neurons[neuron].get_layer()) {
                break;
            }
        }

        int height_spacing = NEURON_HEIGHT;
        if (count) {
            height_spacing /= (count+1);
        }
        else {
            height_spacing = (NEURON_HEIGHT/2) - NEURON_SPACING;
        }

        this->neurons[neuron].position[0] = ((column+1) * width_spacing) - NEURON_RADIUS;
        this->neurons[neuron].position[1] = ((index+1) * height_spacing) - NEURON_RADIUS;
        circle.setPosition(this->neurons[neuron].position[0], this->neurons[neuron].position[1]);
        circles->push_back(circle);

    }
}

// void Genome::draw_neurons(std::vector<sf::CircleShape> *circles) {
//     for (int neuron = 0; neuron < this->neurons.size(); neuron++) {
//         sf::CircleShape circle(NEURON_RADIUS);
//         double value = (this->neurons[neuron].value + 1) / 2;
//         int color = value * 255;
//         circle.setFillColor(sf::Color(255-color, color, 0, 255));
//         circle.setOutlineColor(Colors::node_out_color);
//         circle.setOutlineThickness(3);

//         int width_spacing = NEURON_WIDTH;
//         width_spacing /= MAX_HIDDEN_LAYERS+2;

//         int index = neurons[neuron].get_id();
//         int column = this->neurons[neuron].get_layer();
//         while (index >= MAX_HIDDEN_NODES) {
//             index -= MAX_HIDDEN_NODES;
//         }

//         int height = 0;
//         for (int i = 0; i < this->neurons.size(); i++) {
//             if (this->neurons[i].get_id() >= (column * MAX_HIDDEN_NODES) && this->neurons[i].get_id() < ((column+1) * MAX_HIDDEN_NODES)) {
//                 height++;
//             }
//         }
//         int height_spacing = NEURON_HEIGHT;
//         if (height) {
//             height_spacing /= (height+1);
//         }
//         else {
//             height_spacing = (NEURON_HEIGHT/2) - NEURON_SPACING;
//         }

//         this->neurons[neuron].position[0] = ((column+1) * width_spacing) - NEURON_RADIUS;
//         this->neurons[neuron].position[1] = ((index+1) * height_spacing) - NEURON_RADIUS;
//         circle.setPosition(this->neurons[neuron].position[0], this->neurons[neuron].position[1]);
//         circles->push_back(circle);
//     }
// }

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
            else if (ii > this->neurons.size()-3) {
                end_x = 0;
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