#include "include/BrickBreaker.hpp"

int main(int argc, char** argv)
{
    BrickGroupLayout bricks_layout = {
        60,
        60,
        6,
        6,
        1
    };

    BrickBreaker game = BrickBreaker(bricks_layout); 

    while(game.is_running())
    {
        game.update();
        game.draw();
    }

    return 0;
}