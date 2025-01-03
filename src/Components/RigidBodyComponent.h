#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include <glm.hpp>

struct RigidBodyComponent
{
    glm::vec2 velocity;

    RigidBodyComponent(glm::vec2 velocity = glm::vec2(0.0f))
    {
        this->velocity = velocity;
    }
};

#endif