#include "../include/Ball.hpp"
#include "../include/Collisions.hpp"

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

void Ball::update(float delta_time)
{
    Vector2 movement = move_direction * SPEED * delta_time;

    auto o_info = Collisions::find_collision(get_rectangle(), movement);

    if(!o_info.has_value())
        position += movement;
    else
    {
        auto info = o_info.value();
        position += movement * info.t;

        const Collider& c = Collisions::get_collider(info.id);

        if(c.tag == ColliderTag::OBSTACLE)
        {
            if(abs(info.normal.x) > 0) move_direction.x *= -1; 
            if(abs(info.normal.y) > 0) move_direction.y *= -1;
        }
        else if(c.tag == ColliderTag::PADDLE)
        {
            if(abs(info.normal.x) > 0 || abs(info.normal.y) > 0)
                move_direction = (position - c.get_hitbox().position).normalized();
        }
    }
}