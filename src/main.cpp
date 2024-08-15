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
#include "specimen.hpp"

int main() {
    
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/TTF/Hack-Bold.ttf")) {
        return -1;
    }

    // Create a text object
    sf::Text text;
    text.setFont(font); // Set the font
    text.setString(""); // Set the text string
    char generation[32];
    text.setCharacterSize(15); // Set the character size (in pixels)
    text.setFillColor(sf::Color::White); // Set the text color
    text.setPosition((COLS * CELLSIZE / 2), 1); // Set the position of the text

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

    std::vector<Specimen> genepool;
    int age_counter = 0;
    int generation_counter = 0;
    double best_fitness = 0;
    int max_age = MAX_AGE;
    bool learning = true;
    std::vector<double> fitness_list;

    for (int i = 0; i < MAX_GENES; i++) {
        Specimen item(i);
        genepool.push_back(item);
    }
    Specimen best_specimen = genepool[0];

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
                if (event.key.code == sf::Keyboard::Space) {
                    std::cout << "[";
                    for (int i = 0; i < fitness_list.size(); i++) {
                        std::cout << fitness_list[i] << ",";
                    }
                    std::cout << "]";
                    std::cout << std::endl;
                }
                else if (event.key.code == sf::Keyboard::Enter) {
                    learning = !learning;
                }
            }
        }

        while (genome_window.pollEvent(event)) {
            // Close window : exit
            if (event.type == sf::Event::Closed) {
                genome_window.close();
            }
        }

        // Clear screen
        window.clear();
        genome_window.clear(Colors::genome_background);

        // Get the elapsed time
        sf::Time elapsed = clock.getElapsedTime();

        double outputs[OUTPUT_COUNT];
        // Update snakes
        // if (elapsed > sf::milliseconds(SPEED)) {
        if (learning) {
            clock.restart();

            for (int i = 0; i < genepool.size(); i++) {
                genepool[i].update(&grid);
            }

            age_counter++;
            // std::cout << age_counter << std::endl;
            if (age_counter > max_age) {
                max_age += 10;
                // END
                best_fitness = 0;
                for (int i = 0; i < genepool.size(); i++) {
                    if (genepool[i].get_fitness(max_age) > best_fitness) {
                        best_fitness = genepool[i].get_fitness(max_age);
                    }
                }

                int sorting = 1;
                std::vector<Specimen> genepool_sorted;
                while (sorting) {
                    double last_fitness = 0;
                    int last_index = 0;

                    for (int i = 0; i < genepool.size(); i++) {
                        if (genepool[i].get_fitness(max_age) > last_fitness) {
                            last_fitness = genepool[i].get_fitness(max_age);
                            last_index = i;
                        }
                    }

                    if (genepool.size() <= 0) {
                        break;
                    }

                    genepool_sorted.push_back(genepool[last_index]);

                    std::vector<Specimen>::iterator it = genepool.begin();
                    std::advance(it, last_index);
                    genepool.erase(it);
                }

                genepool = genepool_sorted;
                int keep = genepool.size() * KEEP_RATIO;
                int remove = genepool.size() - keep;

                for (int i = 0; i < remove; i++) {
                    genepool.pop_back();
                }

                for (int i = 0; i < keep; i++) {
                    genepool[i].clear();
                    for (int ii = 0; ii < (remove / keep); ii++) {
                        Specimen item(keep * ii);

                        if (i < keep-1) {
                            item = genepool[i];
                        }

                        item.clear();
                        item.mutate();
                        genepool.push_back(item);
                    }
                }

                age_counter = 0;
                generation_counter++;
                fitness_list.push_back(best_fitness);
            }
        }
        else {
            genepool[0].update(&grid);
        }
        
        genepool[0].draw_specimen(&genome_window);

        // Draw the grid
        drawParameter(&grid, ROWS, COLS);
        for (const auto& cell : grid) {
            window.draw(cell);
        }

        if (learning) {
            for (int i = 0; i < genepool.size(); i++) {
                genepool[i].draw_snake(&window);
            }
        }
        else {
            genepool[0].draw_snake(&window);
        }

        sprintf(generation, "%f - %d - %d", best_fitness, generation_counter, age_counter);
        text.setString(generation);
        window.draw(text);

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