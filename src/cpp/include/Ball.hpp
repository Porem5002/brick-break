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
    void bounce_on_paddle(Rectangle paddle);
    void bounce_inside_container(Rectangle container);
    void bounce_on_obstacle(Rectangle obstacle);
};