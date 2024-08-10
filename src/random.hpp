#pragma once


#include <random>
#include "settings.hpp"

// Create a random number generator
std::random_device rd;  // Obtain a random number from hardware
std::mt19937 generator(rd()); // Seed the generator
std::uniform_int_distribution<int> food_rng(1, ROWS-2); // Define the range