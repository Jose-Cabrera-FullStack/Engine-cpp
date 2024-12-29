#ifndef DAMAGESYSTEM_H
#define DAMAGESYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/HealthComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"

class DamageSystem : public System
{
public:
    DamageSystem()
    {
        RequireComponent<BoxColliderComponent>();
    }

    void SubscribeToEvents(std::unique_ptr<EventBus> &eventBus)
    {
        eventBus->SubscribeToEvent<CollisionEvent>(this, &DamageSystem::OnCollision);
    }

    void OnCollision(CollisionEvent &event)
    {
        Entity a = event.a;
        Entity b = event.b;
        Logger::Log("DamageSystem: Collision between entity " + std::to_string(a.GetId()) + " and entity " + std::to_string(b.GetId()));

        if (a.BelongsToGroup("projectiles") && b.BelongsToGroup("player"))
        {
            OnProjectileHitsPlayer(a, b); // a is the projectile, b is the player
        }
        if (b.BelongsToGroup("projectiles") && a.BelongsToGroup("player"))
        {
            OnProjectileHitsPlayer(b, a); // b is the projectile, a is the player
        }
        if (a.BelongsToGroup("projectiles") && b.BelongsToGroup("enemies"))
        {
        }
        if (b.BelongsToGroup("projectiles") && a.BelongsToGroup("enemies"))
        {
        }
    }

    void OnProjectileHitsPlayer(Entity projectile, Entity player)
    {
        const auto projectileComponent = projectile.GetComponent<ProjectileComponent>();

        if (!projectileComponent.isFriendly)
        {
            // Reduce the health of the player by the projectile hitPercentDamage
            auto &health = player.GetComponent<HealthComponent>();

            // Subtract the health of the player
            health.healthPercentage -= projectileComponent.hitPercentDamage;

            // Kills the player when health reaches zero
            if (health.healthPercentage <= 0)
            {
                player.Kill();
            }

            // Kill the projectile
            projectile.Kill();
        }
    }
    void Update()
    {
    }
};
#endif
