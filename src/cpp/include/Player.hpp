#pragma once

#include <SDL2/SDL.h>

#include "Input.hpp"
#include "Shapes.hpp"

class Player
{
    static constexpr SDL_Color COLOR = { 200, 255, 120, SDL_ALPHA_OPAQUE };
    static constexpr float SPEED = 500;
    static constexpr float WIDTH = 150;
    static constexpr float HEIGHT = 20;

    Vector2 position;
public:
    Player(float x, float y);
    
    Rectangle get_rectangle() const;
    void draw(SDL_Renderer* renderer) const;

    void move(const Input& input, float delta_time);
    void keep_inside_x(Rectangle container);
};