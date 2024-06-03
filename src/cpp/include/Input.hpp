#pragma once

#include <SDL2/SDL.h>

struct InputConfig
{
    SDL_KeyCode proceeed_key;
    SDL_KeyCode move_left_key;
    SDL_KeyCode move_right_key;
};

struct Input
{
    InputConfig config;

    bool proceed;
    bool move_left;
    bool move_right;
    bool quit;
public:
    explicit Input(InputConfig config)
        : config(config), proceed(false), move_left(false), move_right(false), quit(false) { }
    
    void update();

    void clear()
    {
        proceed = false;
    }

    bool should_proceed() const
    {
        return proceed;
    }

    bool should_move_left() const
    {
        return move_left;
    }

    bool should_move_right() const
    {
        return move_right;
    }

    bool should_quit() const
    {
        return quit;
    }
};