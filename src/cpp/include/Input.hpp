#pragma once

#include <SDL2/SDL.h>

struct Input
{
    static constexpr SDL_Keycode PROCEED_KEY = SDLK_SPACE;
    static constexpr SDL_Keycode MOVE_LEFT_KEY = SDLK_a;
    static constexpr SDL_Keycode MOVE_RIGHT_KEY = SDLK_d; 

    bool proceed;
    bool move_left;
    bool move_right;
    bool quit;
public:
    Input() : proceed(false), move_left(false), move_right(false), quit(false) { }
    
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