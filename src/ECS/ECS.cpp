#include <algorithm>
#include "ECS.h"
#include "../Logger/Logger.h"

int Entity::GetId() const
{
    return id;
}

void System::AddEntityToSystem(Entity entity)
{
    entities.push_back(entity);
}
void System::RemoveEntityFromSystem(Entity entity)
{
    entities.erase(
        std::remove_if(
            entities.begin(),
            entities.end(),
            [&entity](const Entity &other)
            { return other == entity; }),
        entities.end());
}
std::vector<Entity> System::GetSystemEntities() const
{
    return entities;
}
const Signature &System::GetComponentSignature() const
{
    return componentSignature;
}

Entity Registry::CreateEntity()
{
    int entityId = numEntities;

    entityId = numEntities++;

    Entity entity(entityId);
    entitiesToBeAdded.insert(entity);

    Logger::Log("Entity created with id: " + std::to_string(entityId));

    return entity;
}

template <typename T, typename... TAgrs>
void Registry::AddComponent(Entity entity, TAgrs &&...args)
{
    const auto componentId = Component<T>::GetId();
    const auto entityId = entity.GetId();

    if (componentId >= componentPools.size())
    {
        componentPools.resize(componentId + 1, nullptr);
    }
    if (!componentPools[componentId])
    {
        Pool<T> *newComponentPool = new Pool<T>();
        componentPools[componentId] = newComponentPool;
    }

    // Get the component pool for the component type.
    Pool<T> *componentPool = Pool<T>(componentPools[componentId]);

    if (entityId >= componentPool->data.size())
    {
        componentPool->data.resize(entityId + 1);
    }

    // Create the component and add it to the pool, and forwad the arguments to the component constructor.
    T newComponent(std::forward<TAgrs>(args)...);

    // Add the component to the pool.
    componentPool->Set(entityId, newComponent);

    // Update the entity signature to include the new component.
    entityComponentSignatures[entityId].set(componentId);
}

void Registry::Update()
{
}