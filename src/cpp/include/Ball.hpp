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
    Ball(Vector2 position, Vector2 move_direction);

    Rectangle get_rectangle() const;
    void draw(SDL_Renderer* renderer) const;

    void move(float delta_time);
    void position_based_deflect_on_collision(Rectangle collider);
    void keep_inside_and_deflect(Rectangle container);
    void keep_outside_and_deflect(Rectangle obstacle);
};