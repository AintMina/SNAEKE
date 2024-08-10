#include "snake.hpp"
#include "colors.hpp"
#include "settings.hpp"
#include "gfx.hpp"


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
    else if (next_cell == Colors::food) {
        this->grow();
        randomPoint(grid);
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