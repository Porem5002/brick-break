#include "../include/Player.hpp"
#include "../include/Collisions.hpp"

Player::Player(float x, float y) : rectangle(x, y, WIDTH, HEIGHT) { }

Player::Player(Vector2 position) : rectangle(position, WIDTH, HEIGHT) { }

void Player::register_collider()
{
    Collisions::add_collider(ColliderTag::PADDLE, &rectangle, [](void*){ });
}
    
Rectangle Player::get_rectangle() const
{
    return rectangle;
}

void Player::draw(SDL_Renderer* renderer) const
{
    SDL_FRect rect = (SDL_FRect)get_rectangle();
    SDL_SetRenderDrawColor(renderer, COLOR.r, COLOR.g, COLOR.b, COLOR.a);
    SDL_RenderFillRectF(renderer, &rect);
}

void Player::move(const Input& input, float delta_time)
{
    float x_direction = 0;

    if(input.should_move_left())
        x_direction--;
    
    if(input.should_move_right())
        x_direction++;

    rectangle.position.x += x_direction * SPEED * delta_time;
}

void Player::keep_inside_x(Rectangle container)
{
    if(rectangle.position.x > container.position.x + container.width/2 - WIDTH/2)
        rectangle.position.x = container.position.x + container.width/2 - WIDTH/2;

    if(rectangle.position.x < container.position.x - container.width/2 + WIDTH/2)
        rectangle.position.x = container.position.x - container.width/2 + WIDTH/2;
}