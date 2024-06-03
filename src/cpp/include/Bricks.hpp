#pragma once

#include <iostream>
#include <vector>

#include <SDL2/SDL.h>

#include "Shapes.hpp"

class Brick
{
    const uint32_t inicial_hits_left;
    uint32_t hits_left;
    Rectangle rectangle;
public:
    Brick(Rectangle rect, int hit_count)
        : inicial_hits_left(hit_count), hits_left(hit_count), rectangle(rect) { }

    bool is_broken() const
    {
        return hits_left == 0;
    }

    void hit()
    {
        if(hits_left > 0) hits_left--;
    }

    Rectangle get_rectangle() const
    {
        return rectangle;
    }

    void draw(SDL_Renderer* renderer) const
    {
        SDL_FRect rect = (SDL_FRect)rectangle;
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRectF(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRectF(renderer, &rect);
    }
};

struct BrickGroupLayout
{
    float padding_horizontal;
    float padding_vertical;

    uint32_t brick_count_horizontal;
    uint32_t brick_count_vertical;

    uint32_t hits_to_break;

    uint32_t get_brick_count() const
    {
        return brick_count_horizontal * brick_count_vertical;
    }

    void generate_bricks_into(Rectangle container, std::vector<Brick>& bricks) const
    {
        float width = (container.width - padding_horizontal*2) / brick_count_horizontal;
        float height = (container.height - padding_vertical*2) / brick_count_vertical;
        Vector2 center = Vector2(0, padding_vertical + height/2);

        for(uint32_t y = 0; y < brick_count_vertical; y++)
        {
            center.x = padding_horizontal + width/2;

            for(uint32_t x = 0; x < brick_count_horizontal; x++)
            {
                Rectangle rect = Rectangle(center, width, height);
                bricks.emplace_back(rect, hits_to_break);
                center.x += width;
            }

            center.y += height;
        }
    }
};