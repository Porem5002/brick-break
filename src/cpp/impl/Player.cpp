#include "../include/Player.hpp"
#include "../include/Collisions.hpp"

Player::Player(float x, float y) : rectangle(x, y, WIDTH, HEIGHT) { }

Player::Player(Vector2 position) : rectangle(position, WIDTH, HEIGHT) { }

void Player::register_collider()
{
    Collisions::add_collider_ref(ColliderTag::PADDLE, &rectangle, [](void*){ });
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

void Player::update(const Input& input, float delta_time)
{
    float x_direction = 0;

    if(input.should_move_left())
        x_direction--;
    
    if(input.should_move_right())
        x_direction++;

    float x_movement = x_direction * SPEED * delta_time;
    Vector2 movement = Vector2(x_movement, 0);

    auto o_info = Collisions::find_collision(get_rectangle(), movement, ColliderTag::OBSTACLE);
    float t = o_info.has_value() ? o_info.value().t : 1.0f;
    rectangle.position += movement * t;
}