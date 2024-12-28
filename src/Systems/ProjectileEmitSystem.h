#ifndef PROJECTILEEMITSYSTEM_H
#define PROJECTILEEMITSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/ProjectileComponent.h"

#include <SDL.h>

class ProjectileEmitSystem : public System
{
public:
    ProjectileEmitSystem()
    {
        RequireComponent<TransformComponent>();
        RequireComponent<ProjectileEmitterComponent>();
    }

    void Update(std::unique_ptr<Registry> &registry)
    {
        for (auto entity : GetSystemEntities())
        {
            auto &projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
            const auto transform = entity.GetComponent<TransformComponent>();

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