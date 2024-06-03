#include "../include/BrickBreaker.hpp"

void BrickBreaker::update()
{
    Rectangle obstacle_rect = Rectangle(700/2.0, 100, 300, 100);

    Uint64 curr_time = SDL_GetTicks();
    float delta_time = (float)(curr_time - prev_time) / 1000;
    prev_time = curr_time;

    input.update();
    
    if(input.should_quit())
    {
        running = false;
        return;
    }

    player.move(input, delta_time);
    player.keep_inside_x(get_screen_rect());

    ball.move(delta_time);
    ball.keep_outside_and_deflect(obstacle_rect);
    ball.position_based_deflect_on_collision(player.get_rectangle());
    ball.keep_inside_and_deflect(get_screen_rect());
}

void BrickBreaker::draw() const
{
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderClear(renderer);

    SDL_FRect rect = (SDL_FRect)Rectangle(700/2.0, 100, 300, 100);

    player.draw(renderer);
    ball.draw(renderer);
    SDL_RenderFillRectF(renderer, &rect);

    SDL_RenderPresent(renderer);
    SDL_Delay(15);
}