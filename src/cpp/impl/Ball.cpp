#include "../include/CollisionSystem.hpp"
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

void Ball::bounce_on_paddle(Rectangle paddle)
{
    if(get_rectangle().colliding_with(paddle))
    {
        position = CollisionSystem::calc_collision_position(get_rectangle(), move_direction, paddle);
        move_direction = (position - paddle.position).normalized();
    }
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

void Ball::bounce_on_obstacle(Rectangle obstacle)
{
    std::optional<CollisionSide> o_collision_side = CollisionSystem::find_collision_side(get_rectangle(), move_direction, obstacle);
    if(!o_collision_side.has_value()) return;

    CollisionSide collision_side = o_collision_side.value();
    Vector2 new_pos = CollisionSystem::calc_collision_position(get_rectangle(), move_direction, obstacle);
    position = new_pos;

    if(collision_side == CollisionSide::UP || collision_side == CollisionSide::DOWN)
        move_direction.y *= -1;
    else if(collision_side == CollisionSide::RIGHT || collision_side == CollisionSide::LEFT)
        move_direction.x *= -1;
}