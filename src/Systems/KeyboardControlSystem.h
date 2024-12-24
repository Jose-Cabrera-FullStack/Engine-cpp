#ifndef KEYBOARDCONTROLSYSTEM_H
#define KEYBOARDCONTROLSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"
#include "../Events/KeyboardEvent.h"

class KeyboardControlSystem : public System
{
public:
    KeyboardControlSystem()
    {
    }

    void SubscribeToEvents(std::unique_ptr<EventBus> &eventBus)
    {
        eventBus->SubscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed);
    }

    void OnKeyPressed(KeyPressedEvent &event)
    {
        std::string keyCode = std::to_string(event.symbol);
        std::string keySymbol(1, (char)event.symbol);

        Logger::Log("Key pressed: " + keyCode + " " + keySymbol);
    }

    void Update()
    {
    }
};

#endif
