#ifndef COLLISSIONEVENT_H
#define COLLISSIONEVENT_H

#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"

class CollisionEvent : public Event
{
public:
    Entity a;
    Entity b;

    CollisionEvent(Entity a, Entity b) : a(a), b(b) {}
};

#endif