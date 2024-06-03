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

    bool hit_brick = false;

    switch(mode)
    {
        case BrickBreakerMode::START:
            if(input.should_proceed())
                mode = BrickBreakerMode::PLAYING;
            break;
        case BrickBreakerMode::PLAYING:
            player.move(input, delta_time);
            player.keep_inside_x(get_screen_rect());

            ball.move(delta_time);

            for(Brick& b : bricks)
            {
                if(!hit_brick && !b.is_broken())
                {
                    if(ball.get_rectangle().colliding_with(b.get_rectangle()))
                    {
                        hit_brick = true;
                        b.hit();

                        if(b.is_broken()) bricks_left--;
                    }

                    ball.keep_outside_and_deflect(b.get_rectangle());
                }
            }

            ball.position_based_deflect_on_collision(player.get_rectangle());

            if(ball.get_rectangle().position.y + ball.get_rectangle().height/2 > get_screen_rect().position.x + get_screen_rect().height/2)
                mode = BrickBreakerMode::FINISHED;
            
            ball.keep_inside_and_deflect(get_screen_rect());

            if(bricks_left == 0)
                mode = BrickBreakerMode::FINISHED;

            break;
        case BrickBreakerMode::FINISHED:
            if(input.should_proceed())
            {
                mode = BrickBreakerMode::START;
                player = Player(WINDOW_WIDTH/2.0, 650);
                ball = Ball(Vector2(WINDOW_HEIGHT/2.0f, 400), Vector2(0, -1));
                load_layout();
            }
            break;
    }

    input.clear();
}

void BrickBreaker::draw() const
{
    SDL_SetRenderDrawColor(renderer, 25, 25, 50, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    player.draw(renderer);

    for(const Brick& b : bricks)
    {
        if(!b.is_broken())
            b.draw(renderer);
    }

    ball.draw(renderer);

    if(mode != BrickBreakerMode::PLAYING)
    {
        SDL_FRect rect = (SDL_FRect)get_screen_rect();
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 60); 
        SDL_RenderFillRectF(renderer, &rect);
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(15);
}