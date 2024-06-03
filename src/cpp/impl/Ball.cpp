#include "../include/Ball.hpp"

Ball::Ball(Vector2 position, Vector2 move_direction) 
        : position(position), move_direction(move_direction) { }

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

void Ball::position_based_deflect_on_collision(Rectangle collider)
{
    if(get_rectangle().colliding_with(collider))
        move_direction = (position - collider.position).normalized();
}

void Ball::keep_inside_and_deflect(Rectangle container)
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

void Ball::keep_outside_and_deflect(Rectangle obstacle)
{
    if(!get_rectangle().colliding_with(obstacle)) return;

    Vector2 diff = (position - obstacle.position).normalized();

    float angle = diff.angle();
    float obs_angle = std::atan2(obstacle.height/2, obstacle.width/2);

    if(angle <= M_PI - obs_angle && angle >= obs_angle)
    {
        position.y = obstacle.position.y + obstacle.height/2 + SIDE/2;
        move_direction.y *= -1;
    }
    else if(angle <= -obs_angle && angle >= -M_PI + obs_angle)
    {
        position.y = obstacle.position.y - obstacle.height/2 - SIDE/2;
        move_direction.y *= -1;
    }
    else if((angle <= obs_angle && angle >= 0) || (angle < 0 && angle >= -obs_angle))
    {
        position.x = obstacle.position.x + obstacle.width/2 + SIDE/2;
        move_direction.x *= -1;
    }
    else
    {
        position.x = obstacle.position.x - obstacle.width/2 - SIDE/2;
        move_direction.x *= -1;
    }
}