#pragma once

#include <SDL2/SDL.h>

#include "Shapes.hpp"

class Ball
{
    static constexpr SDL_Color COLOR = { 255, 255, 255, SDL_ALPHA_OPAQUE };
    static constexpr float SIDE = 20;
    static constexpr float SPEED = 800;

    Vector2 position;
    Vector2 move_direction;
public:
    Ball(Vector2 position, Vector2 move_direction) 
        : position(position), move_direction(move_direction) { }

    void keep_inside_and_deflect(Rectangle container);
    void keep_outside_and_deflect(Rectangle obstacle);

    Rectangle get_rectangle() const
    {
        return Rectangle(position, SIDE, SIDE);
    }

    void move(float delta_time)
    {
        position += move_direction * SPEED * delta_time;
    }

    void position_based_deflect_on_collision(Rectangle collider)
    {
        if(get_rectangle().colliding_with(collider))
            move_direction = (position - collider.position).normalized();
    }

    void draw(SDL_Renderer* renderer) const
    {
        SDL_FRect rect = (SDL_FRect)get_rectangle();
        SDL_SetRenderDrawColor(renderer, COLOR.r, COLOR.g, COLOR.b, COLOR.a);
        SDL_RenderFillRectF(renderer, &rect);
    }
};