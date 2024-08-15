#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <iostream>
#include <random>


#include "gfx.hpp"
#include "colors.hpp"
#include "snake.hpp"
#include "settings.hpp"

#include "link.hpp"
#include "neuron.hpp"
#include "genome.hpp"

int main() {

    // Create genome window
    sf::RenderWindow genome_window(sf::VideoMode(NEURON_WIDTH, NEURON_HEIGHT), "GENOEME");

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(COLS * CELLSIZE, ROWS * CELLSIZE), "SNAEKE");
    sf::Clock clock; // Create a clock to measure time
    sf::Clock frameClock; // Clock to measure frame time
    sf::Image icon;
    icon.loadFromFile("icon.png"); // File/Image/Pixel
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // Create the grid of cells
    std::vector<sf::RectangleShape> grid = createGrid(ROWS, COLS, CELLSIZE);
    // randomPoint(&grid);

    float starting_points[4][2] = {{10, 10}, {ROWS-11, 10}, {10, COLS-10}, {ROWS-11, COLS-10}};
    int dirs[4] = {0, 180, 0, 180};
    std::vector<sf::Color> snake_colors;
    snake_colors.push_back(sf::Color(255, 255, 0, 200));
    snake_colors.push_back(sf::Color(0, 255, 255, 200));
    snake_colors.push_back(sf::Color(255, 165, 0, 200));
    snake_colors.push_back(sf::Color(0, 0, 255, 200));
    std::vector<Snake> players;

    for (int i = 0; i < PLAYER_NUM; i++) {
        Snake player(starting_points[i][0], starting_points[i][1], CELLSIZE, snake_colors[i], dirs[i]);
        players.push_back(player);
    }

    players[0].generate_food();

    Genome gene(0);
    gene.populate();

    // Start the game loop
    while (window.isOpen()) {
        sf::Time frameTime = frameClock.restart(); // Restart the clock and get the elapsed time

        // Process events
        sf::Event event;
        while (window.pollEvent(event)) {
            // Close window : exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::W) {
                    players[0].turn(0, 100);
                }
                else if (event.key.code == sf::Keyboard::S) {
                    players[0].turn(0, -100);
                }
                else if (event.key.code == sf::Keyboard::A) {
                    players[0].turn(-100, 0);
                }
                else if (event.key.code == sf::Keyboard::D) {
                    players[0].turn(100, 0);
                }
                else if (event.key.code == sf::Keyboard::Q) {
                    players[0].turn(-100);
                }
                else if (event.key.code == sf::Keyboard::E) {
                    players[0].turn(100);
                }
                else if (event.key.code == sf::Keyboard::Space) {
                }
                else if (event.key.code == sf::Keyboard::Escape) {
                    for (int i = 0; i < PLAYER_NUM; i++) {
                        players[i].reset(starting_points[i][0], starting_points[i][1], CELLSIZE, snake_colors[i], dirs[i]);
                    }
                }
                else if (PLAYER_NUM > 2) {
                    if (event.key.code == sf::Keyboard::Up) {
                        players[2].turn(0, 100);
                    }
                    else if (event.key.code == sf::Keyboard::Down) {
                        players[2].turn(0, -100);
                    }
                    else if (event.key.code == sf::Keyboard::Left) {
                        players[2].turn(-100, 0);
                    }
                    else if (event.key.code == sf::Keyboard::Right) {
                        players[2].turn(100, 0);
                    }
                }
            }
        }

        while (genome_window.pollEvent(event)) {
            // Close window : exit
            if (event.type == sf::Event::Closed) {
                genome_window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Numpad0) {
                    gene.delete_neuron();
                }
                else if (event.key.code == sf::Keyboard::Numpad1) {
                    gene.delete_link();
                }
                else if (event.key.code == sf::Keyboard::Numpad2) {
                    gene.add_link();
                    // gene.add_neuron();
                }
                else if (event.key.code == sf::Keyboard::Numpad3) {
                    gene.modify_weight();
                }
                else if (event.key.code == sf::Keyboard::Numpad4) {
                    gene.modify_bias();
                }
                else if (event.key.code == sf::Keyboard::Escape) {
                    for (int i = 0; i < PLAYER_NUM; i++) {
                        players[i].reset(starting_points[i][0], starting_points[i][1], CELLSIZE, snake_colors[i], dirs[i]);
                    }
                }
                else if (event.key.code == sf::Keyboard::Enter) {
                    gene.clear();
                    gene.populate();
                }
            }
        }

        // Check if the joystick is connected
        if (sf::Joystick::isConnected(0) && PLAYER_NUM > 1) { // 0 is the index of the joystick
            // Get joystick axis values
            float x = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
            float y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
            players[1].turn(x, -y);

            // Get joystick button states
            bool buttonA = sf::Joystick::isButtonPressed(0, 0); // Button index 0
            if (buttonA) {
                players[1].reset(starting_points[1][0], starting_points[1][1], CELLSIZE, snake_colors[1], dirs[1]);
            }
        }

        // Get the elapsed time
        sf::Time elapsed = clock.getElapsedTime();

        double outputs[OUTPUT_COUNT];
        // Update snakes
        if (elapsed > sf::milliseconds(SPEED)) {
            clock.restart();

            for (int i = 0; i < PLAYER_NUM; i++) {
                if (!sf::Joystick::isConnected(0) && i == 1) {
                    continue;
                }
                players[i].move_snake(&grid);
                players[i].look(&grid);
            }

            if (players[0].is_alive()) {
                // Neural
                uint8_t inputs[INPUT_COUNT];
                inputs[0] = players[0].good.left;
                inputs[1] = players[0].good.forward;
                inputs[2] = players[0].good.right;
                inputs[3] = players[0].bad.left;
                inputs[4] = players[0].bad.forward;
                inputs[5] = players[0].bad.right;

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
                    players[0].turn(-100);
                }
                else if (out == 2) {
                    players[0].turn(100);
                }
            }
            else {
                std::cout << players[0].get_age() << std::endl;
                players[0].reset(starting_points[0][0], starting_points[0][1], CELLSIZE, snake_colors[0], dirs[0]);
                // gene.clear();
                // gene.populate();
                gene.mutate();
            }
        }

        // Clear screen
        window.clear();
        genome_window.clear(Colors::genome_background);

        std::vector<sf::CircleShape> circles;
        gene.draw_neurons(&circles);
        std::vector<sf::VertexArray> links;
        gene.draw_links(&links);
        // Draw links
        for (const auto& link : links) {
            genome_window.draw(link);
        }
        // Draw Neurons
        for (const auto& circle : circles) {
            genome_window.draw(circle);
        }
        

        // Draw the grid
        drawParameter(&grid, ROWS, COLS);
        for (const auto& cell : grid) {
            window.draw(cell);
        }

        // Draw snake
        for (int i = 0; i < PLAYER_NUM; i++) {
            if (!sf::Joystick::isConnected(0) && i == 1) {
                continue;
            }
            players[i].draw_snake(&window);
        }

        // Update the window
        window.display();
        genome_window.display();

        // Calculate time to sleep to cap FPS
        sf::Time sleepTime = sf::seconds(1.0f / FPS) - frameTime;
        if (sleepTime > sf::Time::Zero) {
            sf::sleep(sleepTime);
        }
    }

    return EXIT_SUCCESS;
}