#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"

class CollisionSystem : public System
{
public:
    CollisionSystem()
    {
        RequireComponent<TransformComponent>();
        RequireComponent<BoxColliderComponent>();
    }

    void Update() {
        // AABB collision detection
        //     // for (auto &entity : GetSystemEntities())
        //     // {
        //     //     auto &transform = entity.GetComponent<TransformComponent>();
        //     //     auto &collider = entity.GetComponent<BoxColliderComponent>();

        //     //     collider.offset.x = transform.position.x + collider.offset.x;
        //     //     collider.offset.y = transform.position.y + collider.offset.y;

        //     //     for (auto &other : GetSystemEntities())
        //     //     {
        //     //         if (entity == other)
        //     //         {
        //     //             continue;
        //     //         }

        //     //         auto &otherTransform = other.GetComponent<TransformComponent>();
        //     //         auto &otherCollider = other.GetComponent<BoxColliderComponent>();

        //     //         otherCollider.offset.x = otherTransform.position.x + otherCollider.offset.x;
        //     //         otherCollider.offset.y = otherTransform.position.y + otherCollider.offset.y;

        //     //         if (collider.offset.x + collider.width >= otherCollider.offset.x &&
        //     //             collider.offset.x <= otherCollider.offset.x + otherCollider.width &&
        //     //             collider.offset.y + collider.height >= otherCollider.offset.y &&
        //     //             collider.offset.y <= otherCollider.offset.y + otherCollider.height)
        //     //         {
        //     //             std::cout << "Collision detected!" << std::endl;
        //     //         }
        //     //     }
        // }
    };
};
#endif