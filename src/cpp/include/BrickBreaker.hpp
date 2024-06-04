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
    static constexpr const char* WINDOW_TITLE = "Brick Breaker CPP";
    static constexpr int32_t WINDOW_WIDTH = 700;
    static constexpr int32_t WINDOW_HEIGHT = 700;

    static constexpr float PLAYER_INIT_X = WINDOW_WIDTH/2.0f;
    static constexpr float PLAYER_INIT_Y = 650;

    static constexpr float BALL_INIT_X = WINDOW_HEIGHT/2.0f;
    static constexpr float BALL_INIT_Y = 400;

    static constexpr uint32_t TARGET_FPS = 120;

    Input input;
    Player player;
    Ball ball;

    BrickGroupLayout bricks_layout;
    uint32_t bricks_left;
    std::vector<Brick> bricks;

    SDL_Window* window;
    SDL_Renderer* renderer;
    
    BrickBreakerMode mode;
    Uint64 prev_time;
    bool running;
public:
    BrickBreaker(BrickGroupLayout bricks_layout);
    ~BrickBreaker();

    bool is_running() const;
    Rectangle get_screen_rect() const;
    void draw() const;

    void load_layout();
    void update();
};
