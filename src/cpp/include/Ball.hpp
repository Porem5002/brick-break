#pragma once

#include <SDL2/SDL.h>

#include "Shapes.hpp"

class Ball
{
public:
    static constexpr SDL_Color COLOR = { 255, 255, 255, SDL_ALPHA_OPAQUE };
    static constexpr float SIDE = 20;
    static constexpr float SPEED = 800;

    static constexpr float DIR_INIT_X = 0;
    static constexpr float DIR_INIT_Y = -1;

    Vector2 position;
    Vector2 move_direction;

    Ball(float x, float y);
    Ball(Vector2 position);

    Rectangle get_rectangle() const;
    void draw(SDL_Renderer* renderer) const;

    void move(float delta_time);
    void bounce_on_paddle(Rectangle paddle, Vector2 movement);
    void bounce_inside_container(Rectangle container);
};