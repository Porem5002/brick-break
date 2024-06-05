#include "../include/Ball.hpp"

Ball::Ball(float x, float y) 
        : position(x, y), move_direction(DIR_INIT_X, DIR_INIT_Y) { }

Ball::Ball(Vector2 position) 
        : position(position), move_direction(DIR_INIT_X, DIR_INIT_Y) { }

Rectangle Ball::get_rectangle() const
{
    return Rectangle(position, SIDE, SIDE);
}

void Ball::draw(SDL_Renderer* renderer) const
{
    SDL_FRect rect = (SDL_FRect)get_rectangle();
    SDL_SetRenderDrawColor(renderer, COLOR.r, COLOR.g, COLOR.b, COLOR.a);
    SDL_RenderFillRectF(renderer, &rect);
}

void Ball::move(float delta_time)
{
    position += move_direction * SPEED * delta_time;
}

void Ball::bounce_on_paddle(Rectangle paddle, Vector2 movement)
{
    Vector2 normal;
    float t = SDL_FRect_swept((SDL_FRect)get_rectangle(), (SDL_FRect)paddle, movement, normal);

    position += movement * t;

    if(abs(normal.x) > 0 || abs(normal.y) > 0)
        move_direction = (position - paddle.position).normalized();
}

void Ball::bounce_inside_container(Rectangle container)
{
    if(position.x < container.position.x - container.width/2 + SIDE/2)
    {
        position.x = container.position.x - container.width/2 + SIDE/2;
        move_direction.x *= -1;
    }

    if(position.x > container.position.x + container.width/2 - SIDE/2)
    {
        position.x = container.position.x + container.width/2 - SIDE/2;
        move_direction.x *= -1;
    }

    if(position.y < container.position.y - container.height/2 + SIDE/2)
    {
        position.y = container.position.y - container.height/2 + SIDE/2;
        move_direction.y *= -1;
    }

    if(position.y > container.position.y + container.height/2 - SIDE/2)
    {
        position.y = container.position.y + container.height/2 - SIDE/2;
        move_direction.y *= -1;
    }
}