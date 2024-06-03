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
    Player(float x, float y) : position(Vector2(x, y)) { }
    
    Rectangle get_rectangle() const
    {
        return Rectangle(position, WIDTH, HEIGHT);
    }

    void move(const Input& input, float delta_time)
    {
        float x_direction = 0;

        if(input.should_move_left())
            x_direction--;
        
        if(input.should_move_right())
            x_direction++;

        position.x += x_direction * SPEED * delta_time;
    }

    void keep_inside_x(Rectangle container)
    {
        if(position.x > container.position.x + container.width/2 - WIDTH/2)
            position.x = container.position.x + container.width/2 - WIDTH/2;

        if(position.x < container.position.x - container.width/2 + WIDTH/2)
            position.x = container.position.x - container.width/2 + WIDTH/2;
    }

    void draw(SDL_Renderer* renderer) const
    {
        SDL_FRect rect = (SDL_FRect)get_rectangle();
        SDL_SetRenderDrawColor(renderer, COLOR.r, COLOR.g, COLOR.b, COLOR.a);
        SDL_RenderFillRectF(renderer, &rect);
    }
};