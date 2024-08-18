#include "specimen.hpp"
#include "random.hpp"



Specimen::Specimen() {
    this->id = 0;
    this->snake = Snake(id);
    this->snake.generate_food();
}

Specimen::Specimen(int id) {
    this->id = id;
    this->gene = Genome(id);
    this->gene.populate();
    this->snake = Snake(id);
    this->snake.generate_food();
}

Specimen::Specimen(Genome gene, int id) {
    this->id = id;
    this->gene = gene;
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
        inputs[1] = this->snake.good.forward_left;
        inputs[2] = this->snake.good.forward;
        inputs[3] = this->snake.good.forward_right;
        inputs[4] = this->snake.good.right;
        inputs[5] = this->snake.bad.left;
        inputs[6] = this->snake.bad.forward_left;
        inputs[7] = this->snake.bad.forward;
        inputs[8] = this->snake.bad.forward_right;
        inputs[9] = this->snake.bad.right;
        // inputs[10] = this->snake.get_length();

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

    // Mutate snake color
    int random_effect = (get_neural_rng() * 32);
    int random_value = (get_food_rng() * 2) - 1;
    sf::Color color = this->snake.get_color();


    // Introduce a slight bias
    if (color.r + color.g + color.b < 384) {
        random_value = abs(random_value); // Bias towards increasing color value
    } else {
        random_value = -abs(random_value); // Bias towards decreasing color value
    }

    switch (random_effect) {
        case 0:
            color.r = std::clamp(color.r + random_value, 0, 255);
            break;
        case 1:
            color.g = std::clamp(color.g + random_value, 0, 255);
            break;
        case 2:
            color.b = std::clamp(color.b + random_value, 0, 255);
            break;
        default:
            break;
    }

    this->snake.set_color(color);
}

void Specimen::calculate_fitness(int max_age) {
    double food = this->snake.get_length() - 3;
    double time = this->snake.get_age();

    double temp = 0;
    if (time <= max_age) {
        temp = (time / max_age);
    }
    else {
        temp = 1;
    }

    double value = food;// * temp;
    if (!food) {
        value = 0.01;// * temp;
    }
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

int Specimen::is_alive() {
    return this->snake.is_alive();
}