#ifndef PROJECTILEEMITSYSTEM_H
#define PROJECTILEEMITSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyboardEvent.h"

#include <SDL.h>
#include <glm.hpp>

class ProjectileEmitSystem : public System
{
public:
    ProjectileEmitSystem()
    {
        RequireComponent<TransformComponent>();
        RequireComponent<ProjectileEmitterComponent>();
    }

    void SubscribeToEvents(std::unique_ptr<EventBus> &eventBus)
    {
        eventBus->SubscribeToEvent<KeyPressedEvent>(this, &ProjectileEmitSystem::OnKeyPressed);
    }

    void OnKeyPressed(KeyPressedEvent &event)
    {
        if (event.symbol == SDLK_SPACE)
        {
            for (auto entity : GetSystemEntities())
            {
                if (entity.HasComponent<CameraFollowComponent>())
                {
                    const auto projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
                    const auto transform = entity.GetComponent<TransformComponent>();
                    const auto rigidBody = entity.GetComponent<RigidBodyComponent>();

                    glm::vec2 projectilePosition = transform.position;
                    if (entity.HasComponent<SpriteComponent>())
                    {
                        const auto sprite = entity.GetComponent<SpriteComponent>();
                        projectilePosition.x += (transform.scale.x * sprite.width / 2);
                        projectilePosition.y += (transform.scale.y * sprite.height / 2);
                    }

                    // If parent entity direction is controlled by the keyboard, then the projectile direction should be the same
                    glm::vec2 projectileVelocity = projectileEmitter.projectileVelocity;
                    int directionX = 0;
                    int directionY = 0;
                    if (rigidBody.velocity.x > 0)
                        directionX = +1;
                    if (rigidBody.velocity.x < 0)
                        directionX = -1;
                    if (rigidBody.velocity.y > 0)
                        directionY = +1;
                    if (rigidBody.velocity.y < 0)
                        directionY = -1;
                    projectileVelocity.x = projectileVelocity.x * directionX;
                    projectileVelocity.y = projectileVelocity.y * directionY;

                    // Create new projectile entity and add it to the world
                    Entity projectile = entity.registry->CreateEntity();
                    projectile.Group("projectiles");
                    projectile.AddComponent<TransformComponent>(projectilePosition, glm::vec2(1.0, 1.0), 0.0);
                    projectile.AddComponent<RigidBodyComponent>(projectileVelocity);
                    projectile.AddComponent<SpriteComponent>("bullet-image", 4, 4, 4);
                    projectile.AddComponent<BoxColliderComponent>(4, 4);
                    projectile.AddComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage, projectileEmitter.projectileDuration);
                }
            }
        }
    }

    void Update(std::unique_ptr<Registry> &registry)
    {
        for (auto entity : GetSystemEntities())
        {
            auto &projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
            const auto transform = entity.GetComponent<TransformComponent>();

            // Check if the projectile emitter has a repeat frequency
            if (projectileEmitter.repeatFrequency == 0)
                continue;

            // Check if the time since the last projectile emission is greater than the repeat frequency
            if (SDL_GetTicks() - projectileEmitter.lastEmissionTime > static_cast<Uint32>(projectileEmitter.repeatFrequency))
            {
                glm::vec2 projectilePosition = transform.position;
                if (entity.HasComponent<SpriteComponent>())
                {
                    const auto sprite = entity.GetComponent<SpriteComponent>();
                    projectilePosition.x += (transform.scale.x * sprite.width / 2);
                    projectilePosition.y += (transform.scale.y * sprite.height / 2);
                }
                // Add a new projectile entity to the registry
                auto projectile = registry->CreateEntity();
                projectile.Group("projectiles");
                projectile.AddComponent<TransformComponent>(projectilePosition, glm::vec2(1.0, 1.0), 0.0);
                projectile.AddComponent<RigidBodyComponent>(projectileEmitter.projectileVelocity);
                projectile.AddComponent<SpriteComponent>("bullet-image", 4, 4, 4);
                projectile.AddComponent<BoxColliderComponent>(4, 4);
                projectile.AddComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage, projectileEmitter.projectileDuration);

                // Update the projectile emitter component last emition to the current millisecond
                projectileEmitter.lastEmissionTime = SDL_GetTicks();
            }
        }
    };
};
#endif