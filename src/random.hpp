#pragma once



#include <random>
#include "settings.hpp"


// Function to get a reference to a static random number generator
inline std::mt19937& get_generator() {
    static std::random_device rd; // Obtain a random number from hardware
    static std::mt19937 generator(rd()); // Seed the generator
    return generator;
}

// Function to generate a random integer within the food range
inline int get_food_rng() {
    static std::uniform_int_distribution<int> dist(1, ROWS - 2); // Define the range
    return dist(get_generator());
}

// Function to generate a random double within the neural range
inline double get_neural_rng() {
    static std::uniform_real_distribution<double> dist(0.0, 1.0); // Define the range
    return dist(get_generator());
}