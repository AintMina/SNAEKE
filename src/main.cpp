#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <iostream>
#include <random>


#include "gfx.hpp"
#include "colors.hpp"
#include "snake.hpp"
#include "settings.hpp"

int main() {

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(COLS * CELLSIZE, ROWS * CELLSIZE), "SNAAKE");
    sf::Clock clock; // Create a clock to measure time
    sf::Clock frameClock; // Clock to measure frame time

    // Create the grid of cells
    std::vector<sf::RectangleShape> grid = createGrid(ROWS, COLS, CELLSIZE);
    randomPoint(&grid);

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
                else if (event.key.code == sf::Keyboard::Space) {
                    randomPoint(&grid);
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

        if (elapsed > sf::milliseconds(SPEED)) {
            clock.restart();

            for (int i = 0; i < PLAYER_NUM; i++) {
                if (!sf::Joystick::isConnected(0) && i == 1) {
                    continue;
                }
                players[i].move_snake(&grid);
            }
        }

        // Generates game parameters
        drawParameter(&grid, ROWS, COLS);

        // Clear screen
        window.clear();

        // Draw the grid
        for (const auto& cell : grid) {
            window.draw(cell);
        }

        for (int i = 0; i < PLAYER_NUM; i++) {
            if (!sf::Joystick::isConnected(0) && i == 1) {
                continue;
            }
            players[i].draw_snake(&window);
        }

        // Update the window
        window.display();

        // Calculate time to sleep to cap FPS
        sf::Time sleepTime = sf::seconds(1.0f / FPS) - frameTime;
        if (sleepTime > sf::Time::Zero) {
            sf::sleep(sleepTime);
        }
    }

    return EXIT_SUCCESS;
}