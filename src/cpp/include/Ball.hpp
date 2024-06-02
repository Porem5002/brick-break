#pragma once

#include <SDL2/SDL.h>

#include "Shapes.hpp"

class Ball
{
public:
    Vector2 position;
    Vector2 move_direction;
    float speed = 100;
    float side = 30;

    Ball(Vector2 position, Vector2 move_direction, float speed) 
        : position(position), move_direction(move_direction), speed(speed) { }

    void keep_inside_and_deflect(Rectangle container);
    void keep_outside_and_deflect(Rectangle obstacle);

    Rectangle get_rectangle() const
    {
        return Rectangle(position, side, side);
    }

    void move(float delta_time)
    {
        position += move_direction * speed * delta_time;
    }

    void position_based_deflect_on_collision(Rectangle collider)
    {
        if(get_rectangle().colliding_with(collider))
            move_direction = (position - collider.position).normalized();
    }

    void draw(SDL_Renderer* renderer) const
    {
        SDL_FRect rect = (SDL_FRect)get_rectangle();
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRectF(renderer, &rect);
    }
};