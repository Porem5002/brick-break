#include <optional>

#include "../include/BrickBreaker.hpp"

BrickBreaker::BrickBreaker(BrickGroupLayout bricks_layout)
    : player(PLAYER_INIT_X, PLAYER_INIT_Y), ball(BALL_INIT_X, BALL_INIT_Y),
    bricks_layout(bricks_layout), mode(BrickBreakerMode::START), running(true)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0); 
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    prev_time = SDL_GetTicks64();

    load_layout();
}

BrickBreaker::~BrickBreaker()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool BrickBreaker::is_running() const
{
    return running;
}

Rectangle BrickBreaker::get_screen_rect() const
{
    return Rectangle(WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f, WINDOW_WIDTH, WINDOW_HEIGHT);
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

    uint32_t target_frame_time_ms = (1000 / TARGET_FPS);
    SDL_Delay(target_frame_time_ms);
}

void BrickBreaker::load_layout()
{
    Rectangle rect = get_screen_rect();
    rect.height *= 0.4;

    player = Player(PLAYER_INIT_X, PLAYER_INIT_Y);
    ball = Ball(BALL_INIT_X, BALL_INIT_Y);

    bricks_left = bricks_layout.get_brick_count();
    
    BrickOnHitEvent on_hit(&bricks_left, [](uint32_t* bricks_left, uint32_t hits_left)
    {
        if(hits_left == 0) (*bricks_left)--;
    });

    bricks.clear();
    bricks_layout.generate_bricks_into(rect, bricks, on_hit);
}

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

    switch(mode)
    {
        case BrickBreakerMode::START:
            if(input.should_proceed())
                mode = BrickBreakerMode::PLAYING;
            break;
        case BrickBreakerMode::PLAYING:
        {
            player.move(input, delta_time);
            player.keep_inside_x(get_screen_rect());

            Vector2 movement = ball.move_direction * ball.SPEED * delta_time;

            float t = 1.0f;
            Vector2 normal;
            std::optional<Brick*> final_brick;

            for(Brick& b : bricks)
            {
                if(!b.is_broken())
                {
                    Vector2 n;
                    float curr_t = SDL_FRect_swept((SDL_FRect)ball.get_rectangle(), (SDL_FRect)b.get_rectangle(), movement, n);

                    if(curr_t < t)
                    {
                        normal = n;
                        final_brick = &b;
                        t = curr_t;
                    }
                }
            }

            if(abs(normal.x) > 0 || abs(normal.y) > 0)
            {
                ball.position += movement * t;

                Brick& b = *final_brick.value();
                b.hit();

                if(abs(normal.x) > 0) ball.move_direction.x *= -1; 
                if(abs(normal.y) > 0) ball.move_direction.y *= -1;
            }
            else
                ball.bounce_on_paddle(player.get_rectangle(), movement);

            if(ball.get_rectangle().position.y + ball.get_rectangle().height/2 > get_screen_rect().position.x + get_screen_rect().height/2)
                mode = BrickBreakerMode::FINISHED;
            
            ball.bounce_inside_container(get_screen_rect());

            if(bricks_left == 0)
                mode = BrickBreakerMode::FINISHED;
            
            break;
        }
        case BrickBreakerMode::FINISHED:
            if(input.should_proceed())
            {
                mode = BrickBreakerMode::START;
                load_layout();
            }
            break;
    }

    input.clear();
}