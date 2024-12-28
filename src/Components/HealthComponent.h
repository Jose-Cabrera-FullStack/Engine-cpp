#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

#include <glm.hpp>

struct HealthComponent
{
    int healthPercentage;

    HealthComponent(int healthPercentage = 0)
    {
        this->healthPercentage = healthPercentage;
    }
};

#endif