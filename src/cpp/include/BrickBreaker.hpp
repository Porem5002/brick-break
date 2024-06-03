#pragma once

#include <string>

#include <SDL2/SDL.h>

#include "Player.hpp"
#include "Input.hpp"
#include "Ball.hpp"

class BrickBreaker
{
private:
    Input input;
    Player player = Player(700/2.0, 650);
    Ball ball = Ball(Vector2(700/2.0f, 400), Vector2(0, -1), 700);

    SDL_Window* window;
    SDL_Renderer* renderer;
    
    Uint64 prev_time;
    bool running = true;
public:
    explicit BrickBreaker(InputConfig input_config) : input(input_config)
    {
        SDL_Init(SDL_INIT_EVERYTHING);
        window = SDL_CreateWindow("Brick Breaker CPP", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 700, 700, 0); 
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        prev_time = SDL_GetTicks64();
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
};
