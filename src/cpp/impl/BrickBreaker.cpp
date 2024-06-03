#include "../include/BrickBreaker.hpp"

void BrickBreaker::update()
{
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

    for(Brick& b : bricks)
    {
        if(!b.is_broken())
        {
            if(ball.get_rectangle().colliding_with(b.get_rectangle()))
                b.hit();

            ball.keep_outside_and_deflect(b.get_rectangle());
        }
    }

    ball.position_based_deflect_on_collision(player.get_rectangle());
    ball.keep_inside_and_deflect(get_screen_rect());
}

void BrickBreaker::draw() const
{
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderClear(renderer);

    player.draw(renderer);

    for(const Brick& b : bricks)
    {
        if(!b.is_broken())
            b.draw(renderer);
    }

    ball.draw(renderer);

    SDL_RenderPresent(renderer);
    SDL_Delay(15);
}