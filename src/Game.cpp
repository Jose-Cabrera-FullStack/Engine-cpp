#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <glm.hpp>
#include <iostream>

glm::vec2 playerPosition;
glm::vec2 playerVelocity;

void Game::Setup()
{
    playerPosition = glm::vec2(10.0f, 20.0f);
    playerVelocity = glm::vec2(1.0f, 0.0);
}

Game::Game()
{
    isRunning = false;
    std::cout << "Game constructor" << std::endl;
}

Game::~Game()
{
    std::cout << "Game destructor" << std::endl;
}

void Game::Initialize()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cerr << "Error initializing SDL." << std::endl;
        return;
    }

    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    windowWidth = 800;
    windowHeight = 600;

    window = SDL_CreateWindow(
        "Game Programming in C++ (Chapter 1)", // Window title
        SDL_WINDOWPOS_CENTERED,                // Window x position
        SDL_WINDOWPOS_CENTERED,                // Window y position
        windowWidth,                           // Window width
        windowHeight,                          // Window height
        SDL_WINDOW_BORDERLESS                  // Window flag
    );
    if (!window)
    {
        std::cerr << "Error creating SDL window." << std::endl;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer)
    {
        std::cerr << "Error creating SDL renderer." << std::endl;
        return;
    }

    // SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN); // Fullscreen

    isRunning = true;
}

void Game::Run()
{
    Setup();
    while (isRunning)
    {
        ProcessInput();
        Update();
        Render();
    }
}

void Game::ProcessInput()
{
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent))
    {
        switch (sdlEvent.type)
        {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_KEYDOWN:
            if (sdlEvent.key.keysym.sym == SDLK_ESCAPE)
            {
                isRunning = false;
            }
            break;
        default:
            break;
        }
    }
}

void Game::Update()
{
    while (
        !SDL_TICKS_PASSED(
            SDL_GetTicks(),
            millisecondsPreviousFrame + MILISECONDS_PER_FRAME))
        ;

    millisecondsPreviousFrame = SDL_GetTicks();

    playerPosition.x += playerVelocity.x;
    playerPosition.y += playerVelocity.y;
}

void Game::Render()
{
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);

    SDL_Surface *surface = IMG_Load("../assets/images/tank-tiger-right.png");
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_Rect dstRect = {
        static_cast<int>(playerPosition.x),
        static_cast<int>(playerPosition.x),
        32,
        32};
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
    SDL_DestroyTexture(texture);

    SDL_RenderPresent(renderer);
}

void Game::Destroy()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}