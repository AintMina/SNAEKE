#pragma once


// Grid dimensions
#define ROWS 50
#define COLS 50
#define CELLSIZE 20.0f

#define GRAPH_WIDTH (COLS * CELLSIZE) / 2
#define GRAPH_HEIGHT 300

#define NEURON_RADIUS 10
#define NEURON_SPACING 70
#define NEURON_WIDTH (COLS * CELLSIZE) / 2
#define NEURON_HEIGHT 300

#define INPUT_COUNT 10
#define OUTPUT_COUNT 3
#define MAX_HIDDEN_LAYERS 8
#define MAX_HIDDEN_NODES 11
#define MIN_LINKS INPUT_COUNT + OUTPUT_COUNT

#define SPEED 2
#define PLAYER_NUM 1
#define SNAKE_NUM 0

#define FPS 60.0f
#define MAX_AGE 20000
#define MAX_MAX_AGE 20000
#define MAX_AGE_VEL 0
#define MAX_GENES 30
#define MIN_GENES 30
#define KEEP_RATIO 0.2f
#define TRANSPARENCY 100