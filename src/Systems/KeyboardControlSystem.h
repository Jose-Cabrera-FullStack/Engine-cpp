#ifndef KEYBOARDCONTROLSYSTEM_H
#define KEYBOARDCONTROLSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/SpriteComponent.h"
#include "../Components/KeyboardControlledComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"
#include "../Events/KeyboardEvent.h"

class KeyboardControlSystem : public System
{
public:
    KeyboardControlSystem()
    {
        RequireComponent<KeyboardControlledComponent>();
        RequireComponent<RigidBodyComponent>();
        RequireComponent<SpriteComponent>();
    }

    void SubscribeToEvents(std::unique_ptr<EventBus> &eventBus)
    {
        eventBus->SubscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed);
    }

    void OnKeyPressed(KeyPressedEvent &event)
    {
        for (auto entity : GetSystemEntities())
        {
            const auto keyboardcontrol = entity.GetComponent<KeyboardControlledComponent>();
            auto &rigidbody = entity.GetComponent<RigidBodyComponent>();
            auto &sprite = entity.GetComponent<SpriteComponent>();

            switch (event.symbol)
            {
            case SDLK_w:
                rigidbody.velocity = keyboardcontrol.upVelocity;
                sprite.srcRect.y = sprite.height * 0;
                break;
            case SDLK_d:
                rigidbody.velocity = keyboardcontrol.rightVelocity;
                sprite.srcRect.y = sprite.height * 1;
                break;
            case SDLK_s:
                rigidbody.velocity = keyboardcontrol.downVelocity;
                sprite.srcRect.y = sprite.height * 2;
                break;
            case SDLK_a:
                rigidbody.velocity = keyboardcontrol.leftVelocity;
                sprite.srcRect.y = sprite.height * 3;
                break;
            }
        }
    }

    void Update()
    {
    }
};

#endif
