#include "../include/Input.hpp"

InputData Input::get_updated_input()
{
    SDL_Event event;

    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                data.quit = true;
                break;
            case SDL_KEYDOWN:

                if(event.key.keysym.sym == config.move_left_key)
                    data.move_left = true;
                
                if(event.key.keysym.sym == config.move_right_key)
                    data.move_right = true;

                break;
            case SDL_KEYUP:

                if(event.key.keysym.sym == config.move_left_key)
                    data.move_left = false;
                
                if(event.key.keysym.sym == config.move_right_key)
                    data.move_right = false;

                break;
            default:
                break;
        }
    }

    return data;
}