#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include "../ECS/ECS.h"
#include "../AssetStore/AssetStore.h"
#include "../EventBus/EventBus.h"

const int FPS = 30;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game
{
private:
    bool isRunning;
    bool isDebug;
    int millisecsPreviousFrame = 0;
    SDL_Window *window;
    SDL_Renderer *renderer;

    std::unique_ptr<Registry> registry;
    std::unique_ptr<AssetStore> assetStore;
    std::unique_ptr<EventBus> eventBus;

public:
    Game();
    ~Game();
    void Initialize();
    void Setup();
    void Run();
    void ProcessInput();
    void Update();
    void Render();
    void LoadLevel(int level);
    void Destroy();

    int windowWidth;
    int windowHeight;
};

#endif