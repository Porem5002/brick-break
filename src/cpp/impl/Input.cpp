#include "../include/Input.hpp"

Input::Input() : proceed(false), move_left(false), move_right(false), quit(false) { }

bool Input::should_proceed() const
{
    return proceed;
}

bool Input::should_move_left() const
{
    return move_left;
}

bool Input::should_move_right() const
{
    return move_right;
}

bool Input::should_quit() const
{
    return quit;
}

void Input::update()
{
    SDL_Event event;

    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                if(event.key.keysym.sym == PROCEED_KEY)
                    proceed = true;

                if(event.key.keysym.sym == MOVE_LEFT_KEY)
                    move_left = true;
                
                if(event.key.keysym.sym == MOVE_RIGHT_KEY)
                    move_right = true;

                break;
            case SDL_KEYUP:
                if(event.key.keysym.sym == MOVE_LEFT_KEY)
                    move_left = false;
                
                if(event.key.keysym.sym == MOVE_RIGHT_KEY)
                    move_right = false;

                break;
            default:
                break;
        }
    }
}

void Input::clear()
{
    proceed = false;
}