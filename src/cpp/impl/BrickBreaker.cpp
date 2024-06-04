#include "../include/BrickBreaker.hpp"
#include "../include/CollisionSystem.hpp"

BrickBreaker::BrickBreaker(BrickGroupLayout bricks_layout)
    : player(WINDOW_WIDTH/2.0, 650), ball(Vector2(WINDOW_HEIGHT/2.0f, 400), Vector2(0, -1)),
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

    player = Player(WINDOW_WIDTH/2.0, 650);
    ball = Ball(Vector2(WINDOW_HEIGHT/2.0f, 400), Vector2(0, -1));

    bricks_left = bricks_layout.get_brick_count();
    
    bricks.clear();
    bricks_layout.generate_bricks_into(rect, bricks);
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

                    ball.bounce_on_obstacle(b.get_rectangle());
                }
            }

            ball.bounce_on_paddle(player.get_rectangle());

            if(ball.get_rectangle().position.y + ball.get_rectangle().height/2 > get_screen_rect().position.x + get_screen_rect().height/2)
                mode = BrickBreakerMode::FINISHED;
            
            ball.bounce_inside_container(get_screen_rect());

            if(bricks_left == 0)
                mode = BrickBreakerMode::FINISHED;

            break;
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