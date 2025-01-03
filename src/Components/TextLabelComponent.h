#ifndef TEXTLABELCOMPONENT_H
#define TEXTLABELCOMPONENT_H

#include <string>
#include <glm.hpp>
#include <SDL.h>

struct TextLabelComponent
{
    glm::vec2 position;
    std::string text;
    std::string assetId;
    SDL_Color color;

    bool isFixed;

    TextLabelComponent(
        glm::vec2 position = glm::vec2(0, 0),
        std::string text = "",
        std::string assetId = "charriot-font",
        SDL_Color color = {255, 255, 255, 255},
        bool isFixed = true)
    {
        this->position = position;
        this->text = text;
        this->assetId = assetId;
        this->color = color;
        this->isFixed = isFixed;
    }
};
#endif