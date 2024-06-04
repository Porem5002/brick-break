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

    //MAYBE FIXME: This angle only works for detecting which side of the obstacle we hit
    // because we are running with good FPS, if the delta time was considerably greater then
    // the ball could move inside the obstacle, so the angle will not represent what we are after.
    // But with a more precise calculation of the delta time and multiple updates per frame this should
    // not be a problem (considering the current speed of the ball)
    float angle = diff.angle();
    float obs_angle = std::atan2(obstacle.height/2, obstacle.width/2);

    if(angle <= M_PI - obs_angle && angle >= obs_angle)
    {
        float new_y = obstacle.position.y + obstacle.height/2 + SIDE/2;
        float dir_multiplier = move_direction.y == 0.0 ? 0.0 : (new_y - position.y) / -move_direction.y;

        // Fixing the position since after the collision the ball could overlap with part of the obstacle
        // An equivalent thing is done in the other ifs
        position.y = new_y;
        position.x += -move_direction.x * dir_multiplier;
        move_direction.y *= -1;
    }
    else if(angle <= -obs_angle && angle >= -M_PI + obs_angle)
    {
        float new_y = obstacle.position.y - obstacle.height/2 - SIDE/2;
        float dir_multiplier = move_direction.y == 0.0 ? 0.0 : (new_y - position.y) / -move_direction.y;

        position.y = new_y;
        position.x += -move_direction.x * dir_multiplier;
        move_direction.y *= -1;
    }
    else if((angle <= obs_angle && angle >= 0) || (angle < 0 && angle >= -obs_angle))
    {
        float new_x = obstacle.position.x + obstacle.width/2 + SIDE/2;
        float dir_multiplier = move_direction.x == 0.0 ? 0.0 : (new_x - position.x) / -move_direction.x;

        position.x = new_x;
        position.y += -move_direction.y * dir_multiplier;
        move_direction.x *= -1;
    }
    else
    {
        float new_x = obstacle.position.x - obstacle.width/2 - SIDE/2;
        float dir_multiplier = move_direction.x == 0.0 ? 0.0 : (new_x - position.x) / -move_direction.x;

        position.x = new_x;
        position.y += -move_direction.y * dir_multiplier;
        move_direction.x *= -1;
    }
}