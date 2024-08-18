#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <iostream>
#include <random>


#include <cereal/archives/binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/vector.hpp>
#include <fstream>


#include "gfx.hpp"
#include "colors.hpp"
#include "snake.hpp"
#include "settings.hpp"

#include "link.hpp"
#include "neuron.hpp"
#include "genome.hpp"
#include "specimen.hpp"


// Function to plot the graph
void plot_graph(sf::RenderWindow* window, const std::vector<double>* data) {
    // Margin around the graph
    float margin = 50.0f;
    window->clear();
    
    // Scaling factors to fit the data within the window
    float xScale = (window->getSize().x - 2 * margin) / (data->size() - 1);
    double yMax = *std::max_element(data->begin(), data->end());
    float yScale = (window->getSize().y - 2 * margin) / yMax;

    // Draw the border (offset with margin)
    sf::RectangleShape border(sf::Vector2f(window->getSize().x - 2 * margin, window->getSize().y - 2 * margin));
    border.setPosition(margin, margin);
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineThickness(2);
    border.setOutlineColor(sf::Color(100, 100, 100));
    window->draw(border);

    // Draw grid lines (y-axis)
    for (int i = 0; i <= 10; ++i) {
        float y = margin + i * ((window->getSize().y - 2 * margin) / 10.0f);
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(margin, y), sf::Color(100, 100, 100)),
            sf::Vertex(sf::Vector2f(window->getSize().x - margin, y), sf::Color(100, 100, 100))
        };
        window->draw(line, 2, sf::Lines);
    }

    // Draw grid lines (x-axis)
    for (int i = 0; i <= 10; ++i) {
        float x = margin + i * ((window->getSize().x - 2 * margin) / 10.0f);
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(x, margin), sf::Color(100, 100, 100)),
            sf::Vertex(sf::Vector2f(x, window->getSize().y - margin), sf::Color(100, 100, 100))
        };
        window->draw(line, 2, sf::Lines);
    }

    // Create a vertex array to store the points
    sf::VertexArray lines(sf::LinesStrip, data->size());

    for (std::size_t i = 0; i < data->size(); ++i) {
        float x = margin + i * xScale;
        float y = window->getSize().y - margin - static_cast<float>((*data)[i]) * yScale;
        lines[i].position = sf::Vector2f(x, y);
        lines[i].color = sf::Color::Cyan;
    }

    // Draw the graph
    window->draw(lines);

    // Load font for axis labels
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/TTF/Hack-Bold.ttf")) {
        std::cerr << "Error loading font\n";
    }

    // Draw y-axis labels
    for (int i = 0; i <= 10; ++i) {
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(12);
        text.setFillColor(sf::Color::White);
        double value = yMax * (1.0f - i / 10.0f);
        text.setString(std::to_string(static_cast<int>(value)));
        text.setPosition(margin - 40, margin + i * ((window->getSize().y - 2 * margin) / 10.0f) - 10);
        window->draw(text);
    }

    // Draw x-axis labels
    for (int i = 0; i <= 10; ++i) {
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(12);
        text.setFillColor(sf::Color::White);
        int index = i * (data->size() - 1) / 10;
        text.setString(std::to_string(index));
        text.setPosition(margin + i * ((window->getSize().x - 2 * margin) / 10.0f) - 10, window->getSize().y - margin + 10);
        window->draw(text);
    }
    window->display();
}

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

    // Create graph window
    sf::RenderWindow graph_window(sf::VideoMode(GRAPH_WIDTH, GRAPH_HEIGHT), "GRAEPH");
    graph_window.clear();
    graph_window.display();

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(COLS * CELLSIZE, ROWS * CELLSIZE), "SNAEKE");
    sf::Clock clock; // Create a clock to measure time
    sf::Clock frameClock; // Clock to measure frame time
    sf::Image icon;
    // icon.loadFromFile("icon.png"); // File/Image/Pixel
    // window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

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

                    age_counter = 0;

                    if (learning) {
                        for (int i = 0; i < genepool.size(); i++) {
                            genepool[i].clear();
                        }
                    }
                }
                else if (event.key.code == sf::Keyboard::Escape) {
                    for (int i = 0; i < genepool.size(); i++) {
                        genepool[i].clear();
                    }
                    age_counter = 0;
                }
                else if (event.key.code == sf::Keyboard::Numpad1) {
                    std::ofstream os("out.xml");
                    if (os.is_open()) {
                        cereal::XMLOutputArchive archive(os);
                        archive(genepool[0]);
                    }
                }
                else if (event.key.code == sf::Keyboard::Numpad2) {
                    std::ifstream os("out.xml");
                    if (os.is_open()) {
                        cereal::XMLInputArchive archive(os);
                        Specimen spec;
                        archive(spec);

                        for (int i = 0; i < genepool.size(); i++) {
                            genepool[i] = spec;
                            genepool[i].clear();
                        }
                    }

                    age_counter = 0;
                }
            }
        }

        while (genome_window.pollEvent(event)) {
            // Close window : exit
            if (event.type == sf::Event::Closed) {
                genome_window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Numpad1) {
                    std::ofstream os("out.xml");
                    if (os.is_open()) {
                        cereal::XMLOutputArchive archive(os);
                        archive(genepool[0]);
                    }
                }
                else if (event.key.code == sf::Keyboard::Numpad2) {
                    std::ifstream os("out.xml");
                    if (os.is_open()) {
                        cereal::XMLInputArchive archive(os);
                        Specimen spec;
                        archive(spec);

                        for (int i = 0; i < genepool.size(); i++) {
                            genepool[i] = spec;
                            genepool[i].clear();
                        }
                    }

                    age_counter = 0;
                }
            }
        }

        while (graph_window.pollEvent(event)) {
            // Close window : exit
            if (event.type == sf::Event::Closed) {
                graph_window.close();
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

            int all_dead = 0;
            for (int i = 0; i < genepool.size(); i++) {
                genepool[i].update(&grid);
                if (!genepool[i].is_alive() && all_dead == 0) {
                    all_dead = 0;
                }
                else {
                    all_dead = 1;
                }
            }

            if (!all_dead) {
                age_counter = max_age;
            }

            age_counter++;
            if (age_counter > max_age) {
                // END
                best_fitness = 0;
                for (int i = 0; i < genepool.size(); i++) {
                    if (genepool[i].get_fitness(max_age) > best_fitness) {
                        best_fitness = genepool[i].get_fitness(max_age);
                    }
                }


                // Sort by fitness
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
                int size = genepool.size();

                // If min genes, then keep more
                if (size <= MIN_GENES) {
                    keep = genepool.size() * 0.5;
                }

                // Purge
                int remove = genepool.size() - keep;
                for (int i = 0; i < remove; i++) {
                    genepool.pop_back();
                }

                // Duplicate and add
                for (int i = 0; i < keep; i++) {
                    genepool[i].clear();
                    for (int ii = 0; ii < (remove / keep); ii++) {
                        Specimen item(keep * ii);

                        if (i < keep-1 || size <= MIN_GENES || genepool[i].get_fitness(max_age) > 0.01) {
                            item = genepool[i];
                        }

                        item.clear();
                        item.mutate();
                        genepool.push_back(item);
                    }
                }

                // Remove genes every 10th gen
                if (generation_counter % 10 == 0 && generation_counter > 0 && size > MIN_GENES) {
                    for (int i = 0; i < 5; i++) {
                        genepool.pop_back();
                    }
                }

                // Increment max age
                if (max_age >= MAX_MAX_AGE) {

                }
                else if (size > 20) {
                    max_age += MAX_AGE_VEL;
                }
                else {
                    max_age += MAX_AGE_VEL * 4;
                }

                age_counter = 0;
                generation_counter++;
                fitness_list.push_back(best_fitness);
                plot_graph(&graph_window, &fitness_list);
            }
        }
        else {
            genepool[0].update(&grid);
        }
        
        // Draw the last best specimen genome
        genepool[0].draw_specimen(&genome_window);

        // Draw the grid
        drawParameter(&grid, ROWS, COLS);
        for (const auto& cell : grid) {
            window.draw(cell);
        }

        // Draw snakes
        if (learning) {
            for (int i = 0; i < genepool.size(); i++) {
                genepool[i].draw_snake(&window);
            }
        }
        else {
            genepool[0].draw_snake(&window);
        }

        // Display info
        sprintf(generation, "%f - %d - %d - %d", best_fitness, generation_counter, age_counter, genepool.size());
        text.setString(generation);
        window.draw(text);

        // Update the window
        window.display();
        genome_window.display();

        if (!learning) {
            // Calculate time to sleep to cap FPS
            sf::Time sleepTime = sf::seconds(1.0f / FPS) - frameTime;
            if (sleepTime > sf::Time::Zero) {
                sf::sleep(sleepTime);
            }
        }
    }
    return EXIT_SUCCESS;
}