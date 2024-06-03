#pragma once

#include <iostream>
#include <vector>

#include <SDL2/SDL.h>

#include "Shapes.hpp"

class Brick
{
    static constexpr SDL_Color FILL_COLOR = { 200, 170, 170, SDL_ALPHA_OPAQUE };
    static constexpr SDL_Color BORDER_COLOR = { 0, 0, 0, SDL_ALPHA_OPAQUE };

    const uint32_t inicial_hits_left;
    uint32_t hits_left;
    Rectangle rectangle;
public:
    Brick(Rectangle rect, int hit_count);

    bool is_broken() const;
    Rectangle get_rectangle() const;
    void draw(SDL_Renderer* renderer) const;

    void hit();
};

struct BrickGroupLayout
{
    float padding_horizontal;
    float padding_vertical;

    uint32_t brick_count_horizontal;
    uint32_t brick_count_vertical;

    uint32_t hits_to_break;

    uint32_t get_brick_count() const;
    void generate_bricks_into(Rectangle container, std::vector<Brick>& bricks) const;
};