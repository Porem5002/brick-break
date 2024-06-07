#include "../include/BrickBreaker.hpp"
#include "../include/Collisions.hpp"

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
    Collisions::clear();

    Rectangle rect = get_screen_rect();
    rect.height *= 0.4;

    player = Player(PLAYER_INIT_X, PLAYER_INIT_Y);
    ball = Ball(BALL_INIT_X, BALL_INIT_Y);

    BrickOnHitEvent on_hit(this, [](BrickBreaker* me, uint32_t hits_left)
    {
        if(hits_left == 0) me->bricks_left--;

        if(me->bricks_left == 0) me->mode = BrickBreakerMode::FINISHED;
    });

    bricks_layout.generate_bricks_into(rect, bricks, on_hit);
    bricks_left = bricks_layout.get_brick_count();

    register_colliders();
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
            player.update(input, delta_time);
            ball.update(delta_time);
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

void BrickBreaker::register_colliders()
{
    Collisions::clear();
    
    player.register_collider();

    for(auto& b : bricks) b.register_collider();

    constexpr float BARRIER_THICKNESS = 0.1f;

    Rectangle screen_rect = get_screen_rect();
    
    Vector2 right = screen_rect.position + Vector2(screen_rect.width/2 - BARRIER_THICKNESS/2, 0);
    Vector2 left = screen_rect.position + Vector2(-screen_rect.width/2 + BARRIER_THICKNESS/2, 0);
    Vector2 up = screen_rect.position + Vector2(0, -screen_rect.height/2 + BARRIER_THICKNESS/2);

    Collisions::add_collider(ColliderTag::OBSTACLE, Rectangle(right, BARRIER_THICKNESS, screen_rect.height), [](void*){ });
    Collisions::add_collider(ColliderTag::OBSTACLE, Rectangle(left, BARRIER_THICKNESS, screen_rect.height), [](void*){ });
    Collisions::add_collider(ColliderTag::OBSTACLE, Rectangle(up, screen_rect.width, BARRIER_THICKNESS), [](void*){ });

    ColliderOnHitEvent stop_game(&mode, [](BrickBreakerMode* mode) {
        *mode = BrickBreakerMode::FINISHED;
    });

    Vector2 down = screen_rect.position + Vector2(0, screen_rect.height/2 - BARRIER_THICKNESS/2);
    Collisions::add_collider(ColliderTag::OBSTACLE, Rectangle(down, screen_rect.width, BARRIER_THICKNESS), stop_game);
}