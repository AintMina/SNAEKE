#include "snake.hpp"
#include "colors.hpp"
#include "settings.hpp"
#include "gfx.hpp"
#include "random.hpp"


Snake::Snake(float x, float y, float cell_size, sf::Color color, int dir) {
    this->color = color;
    this->cell_size = cell_size;
    this->head_coords[0] = x;
    this->head_coords[1] = y;
    this->dir = dir;
    sf::RectangleShape start_segment(sf::Vector2f(cell_size, cell_size));
    start_segment.setFillColor(Colors::snake_color);
    start_segment.setPosition(x*cell_size, y*cell_size);

    this->head = start_segment;
    this->body.push_back(start_segment);

    sf::Vector2f offset(0, 0);

    if (this->dir == 0) {
        offset.x -= this->cell_size;
    }
    else if (this->dir == 180) {
        offset.x += this->cell_size;
    }
    else if (this->dir == 90) {
        offset.y -= this->cell_size;
    }
    else if (this->dir == 270) {
        offset.y += this->cell_size;
    }

    sf::RectangleShape body_segment(sf::Vector2f(cell_size, cell_size));
    body_segment.setFillColor(Colors::snake_color);
    
    for (int i = 0; i < this->length; i++) {
        body_segment.setPosition((x*cell_size) + (offset.x * (i+1)), (y*cell_size) + (offset.y * (i+1)));
        this->body.push_back(body_segment);
    }

}

void Snake::reset(float x, float y, float cell_size, sf::Color color, int dir) {
    this->body.clear();
    this->length = 3;
    this->alive = 1;
    this->age = 0;
    this->color = color;
    this->cell_size = cell_size;
    this->head_coords[0] = x;
    this->head_coords[1] = y;
    this->dir = dir;
    sf::RectangleShape start_segment(sf::Vector2f(cell_size, cell_size));
    start_segment.setFillColor(Colors::snake_color);
    start_segment.setPosition(x*cell_size, y*cell_size);

    this->head = start_segment;
    this->body.push_back(start_segment);

    sf::Vector2f offset(0, 0);

    if (this->dir == 0) {
        offset.x -= this->cell_size;
    }
    else if (this->dir == 180) {
        offset.x += this->cell_size;
    }
    else if (this->dir == 90) {
        offset.y -= this->cell_size;
    }
    else if (this->dir == 270) {
        offset.y += this->cell_size;
    }

    sf::RectangleShape body_segment(sf::Vector2f(cell_size, cell_size));
    body_segment.setFillColor(Colors::snake_color);
    
    for (int i = 0; i < this->length; i++) {
        body_segment.setPosition((x*cell_size) + (offset.x * (i+1)), (y*cell_size) + (offset.y * (i+1)));
        this->body.push_back(body_segment);
    }
}

int Snake::move_snake(std::vector<sf::RectangleShape> *grid) {

    if (!this->alive) {
        return -1;
    }

    this->age++;

    sf::Vector2f offset(0, 0);
    int coord_offset[2] = {0, 0};

    if (this->dir == 0) {
        offset.x += this->cell_size;
        coord_offset[0]++;
    }
    else if (this->dir == 180) {
        offset.x -= this->cell_size;
        coord_offset[0]--;
    }
    else if (this->dir == 90) {
        offset.y += this->cell_size;
        coord_offset[1]++;
    }
    else if (this->dir == 270) {
        offset.y -= this->cell_size;
        coord_offset[1]--;
    }

    // Get next cell
    sf::Color next_cell = get_next_cell(grid, coord_offset);

    if (next_cell == Colors::border) {
        this->alive = 0;
        this->color.a = 120;
        return -1;
    }
    else if (this->head_coords[0] + coord_offset[0] == this->food[0] && this->head_coords[1] + coord_offset[1] == this->food[1]) {
        this->grow();
        this->generate_food();
    }

    for (int i = 0; i < this->length; i++) {
        // If hit itself
        if (this->head.getPosition().x + offset.x == this->body[i].getPosition().x) {
            if (this->head.getPosition().y + offset.y == this->body[i].getPosition().y) {
                this->alive = 0;
                this->color.a = 120;
                return -1;
            }
        }

        if (i == 0 && this->grow_flag) {
            if (this->grow_flag) {
                this->body.push_back(this->body[(this->length-1)]);
                this->grow_flag = 0;
                this->length++;
            }
        }
        else {
            this->body[(this->length) - i] = this->body[(this->length-1) - i];
        }
    }

    this->head_coords[0] += coord_offset[0];
    this->head_coords[1] += coord_offset[1];
    this->head.move(offset);
    this->body[0] = head;
    (*grid)[((this->head_coords[1]) * COLS) + (this->head_coords[0])].setFillColor(Colors::background);

    this->turning_flag = 0;

    return 0;
}

void Snake::draw_snake(sf::RenderWindow *window) {
    for (int i = 0; i < this->length; i++) {
        this->body[i].setFillColor(this->color);
        window->draw(this->body[i]);
    }

    this->draw_food(window);
}

void Snake::draw_food(sf::RenderWindow *window) {
    sf::RectangleShape food(sf::Vector2f(this->cell_size, this->cell_size));
    food.setPosition(this->food[0] * this->cell_size, this->food[1] * this->cell_size);
    food.setFillColor(Colors::food); // Default color
    window->draw(food);
}

 void Snake::turn(float x, float y) {
    if (!this->turning_flag) {
        if (x < -50 && this->dir != 0) {
            this->dir = 180;
        }
        else if (x > 50 && this->dir != 180) {
            this->dir = 0;
        }
        else if (y < -50 && this->dir != 270) {
            this->dir = 90;
        }
        else if (y > 50 && this->dir != 90) {
            this->dir = 270;
        }
        this->turning_flag = 1;
    }
}

 void Snake::turn(float x) {
    if (!this->turning_flag) {
        if (x < -50) {
            this->dir -= 90;
        }
        else if (x > 50) {
            this->dir += 90;
        }
        this->turning_flag = 1;
    }

    if (this->dir >= 360) {
        this->dir -= 360;
    }
    else if (this->dir < 0) {
        this->dir += 360;
    }
}

void Snake::grow() {
    this->grow_flag = 1;
}


sf::Color Snake::get_next_cell(std::vector<sf::RectangleShape> *grid, int offset[2]) {
    return (*grid)[((this->head_coords[1] + offset[1]) * COLS) + (this->head_coords[0] + offset[0])].getFillColor();
}


void Snake::look(std::vector<sf::RectangleShape> *grid) {
    
    this->bad.forward = 0;
    this->bad.left = 0;
    this->bad.right = 0;
    this->good.forward = 0;
    this->good.left = 0;
    this->good.right = 0;

    uint8_t found_good = 0;
    uint8_t found_bad = 0;

    // Forward
    for (int i = 1; i <= COLS; i++) {

        uint8_t x = 0;
        uint8_t y = 0;

        if (this->dir == 0) {
            x += i;
        }
        else if (this->dir == 90) {
            y += i;
        }
        else if (this->dir == 180) {
            x -= i;
        }
        else if (this->dir == 270) {
            y -= i;
        }

        x += this->head_coords[0];
        y += this->head_coords[1];

        for (int ii = 0; ii < this->length; ii++) {
            if (this->body[ii].getPosition().x == (x * this->cell_size) && this->body[ii].getPosition().y == (y * this->cell_size)) {
                found_bad = 1;
                break;
            }
        }

        sf::Color color = (*grid)[x + (y * COLS)].getFillColor();

        if (x == this->food[0] && y == this->food[1]) {
            found_good = 1;
        }
        else if (color == Colors::border) {
            found_bad = 1;
        }
        if (color == Colors::background) {
            if (!found_bad) {
                this->bad.forward++;
            }
            if (!found_good) {
                this->good.forward++;
            }
        }

        if (found_bad && !found_good) {
            this->good.forward = 255;
            break;
        }
        else if (!found_bad && found_good) {
            this->bad.forward = 255;
            break;
        }
    }
    found_good = 0;
    found_bad = 0;

    // Left
    for (int i = 1; i <= COLS; i++) {

        uint8_t x = 0;
        uint8_t y = 0;

        if (this->dir == 0) {
            y += i;
        }
        else if (this->dir == 90) {
            x -= i;
        }
        else if (this->dir == 180) {
            y -= i;
        }
        else if (this->dir == 270) {
            x += i;
        }

        x += this->head_coords[0];
        y += this->head_coords[1];

        for (int ii = 0; ii < this->length; ii++) {
            if (this->body[ii].getPosition().x == (x * this->cell_size) && this->body[ii].getPosition().y == (y * this->cell_size)) {
                found_bad = 1;
                break;
            }
        }

        sf::Color color = (*grid)[x + (y * COLS)].getFillColor();

        if (x == this->food[0] && y == this->food[1]) {
            found_good = 1;
        }
        else if (color == Colors::border) {
            found_bad = 1;
        }
        if (color == Colors::background) {
            if (!found_bad) {
                this->bad.left++;
            }
            if (!found_good) {
                this->good.left++;
            }
        }

        if (found_bad && !found_good) {
            this->good.left = 255;
            break;
        }
        else if (!found_bad && found_good) {
            this->bad.left = 255;
            break;
        }
    }
    found_good = 0;
    found_bad = 0;

    // Right
    for (int i = 1; i <= COLS; i++) {

        uint8_t x = 0;
        uint8_t y = 0;

        if (this->dir == 0) {
            y -= i;
        }
        else if (this->dir == 90) {
            x += i;
        }
        else if (this->dir == 180) {
            y += i;
        }
        else if (this->dir == 270) {
            x -= i;
        }

        x += this->head_coords[0];
        y += this->head_coords[1];

        for (int ii = 0; ii < this->length; ii++) {
            if (this->body[ii].getPosition().x == (x * this->cell_size) && this->body[ii].getPosition().y == (y * this->cell_size)) {
                found_bad = 1;
                break;
            }
        }

        sf::Color color = (*grid)[x + (y * COLS)].getFillColor();

        if (x == this->food[0] && y == this->food[1]) {
            found_good = 1;
        }
        else if (color == Colors::border) {
            found_bad = 1;
        }
        else if (color == Colors::background) {
            if (!found_bad) {
                this->bad.right++;
            }
            if (!found_good) {
                this->good.right++;
            }
        }

        if (found_bad && !found_good) {
            this->good.right = 255;
            break;
        }
        else if (!found_bad && found_good) {
            this->bad.right = 255;
            break;
        }
    }
}

uint8_t Snake::is_alive() {
    return this->alive;
}

void Snake::generate_food() {
    int x = get_food_rng();
    int y = get_food_rng();

    this->food[0] = x;
    this->food[1] = y;
}

int Snake::get_age() {
    return this->age;
}