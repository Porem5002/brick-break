#pragma once

#include <SDL2/SDL.h>

#include "Input.hpp"
#include "Shapes.hpp"

class Player
{
    Vector2 position;
    
    SDL_Color color = { 200, 255, 120, 255 };
    float speed = 500;
    float width = 150;
    float height = 20;
public:
    Player(float x, float y) : position(Vector2(x, y)) { }
    
    Rectangle get_rectangle() const
    {
        return Rectangle(position, width, height);
    }

    void move(const Input& input, float delta_time)
    {
        float x_direction = 0;

        if(input.should_move_left())
            x_direction--;
        
        if(input.should_move_right())
            x_direction++;

        position.x += x_direction * speed * delta_time;
    }

    void keep_inside_x(Rectangle container)
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