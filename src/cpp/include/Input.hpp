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
    Input();

    bool should_proceed() const;
    bool should_move_left() const;
    bool should_move_right() const;
    bool should_quit() const;

    void update();
    void clear();
};