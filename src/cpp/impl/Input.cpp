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
                if(event.key.keysym.sym == config.proceeed_key)
                    proceed = true;

                if(event.key.keysym.sym == config.move_left_key)
                    move_left = true;
                
                if(event.key.keysym.sym == config.move_right_key)
                    move_right = true;

                break;
            case SDL_KEYUP:
                if(event.key.keysym.sym == config.move_left_key)
                    move_left = false;
                
                if(event.key.keysym.sym == config.move_right_key)
                    move_right = false;

                break;
            default:
                break;
        }
    }
}