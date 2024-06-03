#include "include/BrickBreaker.hpp"

int main(int argc, char** argv)
{
    InputConfig input_config = {
        .move_left_key = SDLK_a,
        .move_right_key = SDLK_d,
    };

    BrickGroupLayout bricks_layout = {
        60,
        60,
        8,
        4,
        2
    };

    BrickBreaker game = BrickBreaker(input_config, bricks_layout); 

    while(game.is_running())
    {
        game.update();
        game.draw();
    }

    return 0;
}