#include "specimen.hpp"



Specimen::Specimen(int id) {
    this->id = id;
    this->gene = Genome(id);
    this->gene.populate();
    this->snake = Snake(id);
    this->snake.generate_food();
}

void Specimen::clear() {
    this->fitness = 0;
    this->calculated = false;
    this->snake.reset();
}

int Specimen::get_id() const {
    return this->id;
}

void Specimen::set_id(int new_id) {
    this->id = new_id;
}

void Specimen::update(std::vector<sf::RectangleShape> *grid) {
    if (this->snake.is_alive()) {
        this->snake.move_snake(grid);
        this->snake.look(grid);

        double outputs[OUTPUT_COUNT];
        uint8_t inputs[INPUT_COUNT];
        inputs[0] = this->snake.good.left;
        inputs[1] = this->snake.good.forward;
        inputs[2] = this->snake.good.right;
        inputs[3] = this->snake.bad.left;
        inputs[4] = this->snake.bad.forward;
        inputs[5] = this->snake.bad.right;

        double input_values[INPUT_COUNT];
        for (int ii = 0; ii < INPUT_COUNT; ii++) {
            if (inputs[ii] == 255) {
                input_values[ii] = 1.0;
                continue;
            }
            input_values[ii] = ((static_cast<double>(inputs[ii]) / (ROWS / 2.0)) - 1.0);
        }

        gene.set_input(input_values);
        gene.update_network();
        gene.get_output(outputs);
        gene.calculated();

        int out = 0;
        double old = 0.0;
        for (int i = 0; i < OUTPUT_COUNT; i++) {
            if (outputs[i] > old) {
                old = outputs[i];
                out = i;
            }
        }

        if (out == 0) {
            this->snake.turn(-100);
        }
        else if (out == 2) {
            this->snake.turn(100);
        }
    }
    else {

    }
}

void Specimen::mutate() {
    this->gene.mutate();
}

void Specimen::calculate_fitness(int max_age) {
    double food = this->snake.get_length() - 3;
    double time = this->snake.get_age();

    double temp = (time / max_age);
    double value = food * temp;
    this->fitness = value;
    this->calculated = true;
    this->age++;
}

double Specimen::get_fitness(int max_age) {
    if (this->calculated) {
        return this->fitness;
    }
    else {
        this->calculate_fitness(max_age);
    }

    return this->fitness;
}

void Specimen::draw_specimen(sf::RenderWindow *genome_window) {
    std::vector<sf::CircleShape> circles;
    gene.draw_neurons(&circles);
    std::vector<sf::VertexArray> links;
    gene.draw_links(&links);
    // Draw links
    for (const auto& link : links) {
        genome_window->draw(link);
    }
    // Draw Neurons
    for (const auto& circle : circles) {
        genome_window->draw(circle);
    }
}

void Specimen::draw_snake(sf::RenderWindow *window) {
    this->snake.draw_snake(window);
}