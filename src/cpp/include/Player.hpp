#pragma once

#include <SDL2/SDL.h>

#include "Shapes.hpp"

struct Player
{
    Vector2 position;
    
    SDL_Color color = { 255, 255, 255, 255 };
    float speed = 500;
    float width = 200;
    float height = 50;

    float x_direction = 0;

    Player(float x, float y) : position(Vector2(x, y)) { }
    
    Rectangle get_rectangle() const
    {
        return Rectangle(position, width, height);
    }

    void move(float delta_time)
    {
        position.x += x_direction * speed * delta_time;
    }

    void keep_inside_horizontal(Rectangle container)
    {
        if(position.x > container.position.x + container.width/2 - width/2)
            position.x = container.position.x + container.width/2 - width/2;

        if(position.x < container.position.x - container.width/2 + width/2)
            position.x = container.position.x - container.width/2 + width/2;
    }

    void draw(SDL_Renderer* renderer) const
    {
        SDL_FRect rect = (SDL_FRect)get_rectangle();
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRectF(renderer, &rect);
    }
};