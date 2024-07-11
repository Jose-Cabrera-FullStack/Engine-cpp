#include <iostream>
#include <SDL.h>

int main(int argc, char *argv[]) // Update the signature of main
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    std::cout << "Hello, World!" << std::endl;

    SDL_Quit();
    return 0;
}
