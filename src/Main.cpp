#include <SDL.h>
#include "Game.h"
#include <iostream>

int main(int argc, char *argv[])
{
    std::cout << "Prueba main" << std::endl; // Línea de prueba

    Game game;

    game.Initialize();
    game.Run();
    game.Destroy();

    return 0;
}
