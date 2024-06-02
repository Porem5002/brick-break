#pragma once

#include <SDL2/SDL.h>

struct InputConfig
{
    SDL_KeyCode move_left_key;
    SDL_KeyCode move_right_key;
};

struct InputData
{
    bool move_left = false;
    bool move_right = false;
    bool quit = false;
};

struct Input
{
    InputConfig config;
    InputData data;

public:
    Input(InputConfig config) : config(config) { }
    InputData get_updated_input();
};