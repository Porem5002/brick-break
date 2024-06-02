#include "include/BrickBreaker.hpp"

int main(int argc, char** argv)
{
    InputConfig input_config = {
        .move_left_key = SDLK_a,
        .move_right_key = SDLK_d,
    };

    BrickBreaker game = BrickBreaker(input_config); 

    while(game.is_running())
    {
        game.update();
        game.draw();
    }

    return 0;
}