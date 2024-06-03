#include "../include/Input.hpp"

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