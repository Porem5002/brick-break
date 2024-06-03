#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <SDL2/SDL.h>

#include "Player.hpp"
#include "Input.hpp"
#include "Ball.hpp"
#include "Bricks.hpp"

enum BrickBreakerMode : uint32_t
{
    START,
    PLAYING,
    FINISHED,
};

class BrickBreaker
{
    Input input;
    Player player = Player(700/2.0, 650);
    Ball ball = Ball(Vector2(700/2.0f, 400), Vector2(0, -1));

    BrickGroupLayout bricks_layout;
    uint32_t bricks_left;
    std::vector<Brick> bricks;

    SDL_Window* window;
    SDL_Renderer* renderer;
    
    BrickBreakerMode mode;
    Uint64 prev_time;
    bool running;
public:
    BrickBreaker(InputConfig input_config, BrickGroupLayout bricks_layout)
        : input(input_config), bricks_layout(bricks_layout), mode(BrickBreakerMode::START), running(true)
    {
        SDL_Init(SDL_INIT_EVERYTHING);
        window = SDL_CreateWindow("Brick Breaker CPP", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 700, 700, 0); 
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

        prev_time = SDL_GetTicks64();

        load_layout();
    }

    ~BrickBreaker()
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void update();
    void draw() const;

    bool is_running() const
    {
        return running;
    }

    Rectangle get_screen_rect() const
    {
        return Rectangle(700/2.0f, 700/2.0f, 700, 700);
    }

    void load_layout()
    {
        Rectangle rect = get_screen_rect();
        rect.height *= 0.4;

        bricks_left = bricks_layout.get_brick_count();

        bricks.clear();
        bricks_layout.generate_bricks_into(rect, bricks);
    }
};
