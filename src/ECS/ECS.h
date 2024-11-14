#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <set>

const unsigned int MAX_COMPONENTS = 32;

//////////////////////////////////////////////////////////////////////////////////////////
// Signature
//////////////////////////////////////////////////////////////////////////////////////////
// We use a bitset (1s and Os) to keep track of which components an entity has,
// and also helps keep track of which entities a system is interested in.
//////////////////////////////////////////////////////////////////////////////////////////
typedef std::bitset<MAX_COMPONENTS> Signature;

struct IComponent
{
protected:
    static int nextId;
};

template <typename TComponent>
class Component : public IComponent
{
    static int GetId()
    {
        static int id = nextId++;
        return id;
    }
};

class Entity
{
private:
    int id;

public:
    Entity(int id) : id(id) {};
    int GetId() const;

    Entity &operator=(const Entity &other) = default;

    bool operator==(const Entity &other) const
    {
        return id == other.id;
    }

    bool operator!=(const Entity &other) const
    {
        return id != other.id;
    }

    bool operator<(const Entity &other) const
    {
        return id < other.id;
    }

    bool operator>(const Entity &other) const
    {
        return id > other.id;
    }
};

//////////////////////////////////////////////////////////////////////////////////////////
// System
//////////////////////////////////////////////////////////////////////////////////////////
// The system processes the entities that contain a specific signature.
//////////////////////////////////////////////////////////////////////////////////////////
class System
{
private:
    Signature componentSignature;
    std::vector<Entity> entities;

public:
    System() = default;
    ~System() = default;

    void AddEntityToSystem(Entity entity);
    void RemoveEntityFromSystem(Entity entity);
    std::vector<Entity> GetSystemEntities() const;
    const Signature &GetComponentSignature() const;

    template <typename TComponent>
    void RequireComponent();
};

//////////////////////////////////////////////////////////////////////////////////////////
// Pool
//////////////////////////////////////////////////////////////////////////////////////////
// A pool is just a vector (contiguous data) of objects of type T.
//////////////////////////////////////////////////////////////////////////////////////////
class IPool
{
public:
    virtual ~IPool() = default;
};

template <typename T>
class Pool : IPool
{
private:
    std::vector<T> data;

public:
    virtual Pool(int size = 100) = {
        data.resize(size);
}

~Pool() = default;

bool isEmtpy() const
{
    return data.empty();
}

int GetSize() const
{
    return data.size();
}

void Resize(int size)
{
    data.resize(size);
}

void Clear()
{
    data.clear();
}

void Add(const T &component)
{
    data.push_back(component);
}

void Set(int index, const T &component)
{
    data[index] = component;
}

T &Get(int index)
{
    return static_cast<T &>(data[index]);
};

T &operator[](unsigned int index)
{
    return data[index];
}
}
;
//////////////////////////////////////////////////////////////////////////////////////////
// Registry
//////////////////////////////////////////////////////////////////////////////////////////
// The registry manages the creation and destruction of entities, add systems,
// and add components to entities.
//////////////////////////////////////////////////////////////////////////////////////////

class Registry
{
private:
    int numEntities = 0;

    // Vector of component pools, each pool contains all the data for a certain component type.
    // Vector index = component type id
    // Pool index = entity id
    std::vector<IPool *> componentPools;

    std::vector<Signature> entityComponentSignatures;

    std::unordered_map<std::type_index, System *> systems;

    // Set of entities that are flagged to be added or removed from the registry Update loop.
    std::set<Entity> entitiesToBeAdded;
    std::set<Entity> entitiesToBeRemoved;

public:
    Registry() = default;

    // The registry Update() finally processes the entities that are flagged to be added or removed.
    void Update();

    // Entity management
    Entity CreateEntity();

    // Component management
    template <typename TComponent, typename... TAgrs>
    void AddComponent(Entity entity, TAgrs &&...args);
    template <typename TComponent>
    void RemoveComponent(Entity entity);
    template <typename TComponent>
    bool HasComponent(Entity entity) const;

    // System management
    template <typename TSystem, typename... TArgs>
    void AddSystem(TArgs &&...args);
    template <typename TSystem>
    void RemoveSystem();
    template <typename TSystem>
    bool HasSystem() const;
    template <typename TSystem>
    TSystem &GetSystem() const;

    // Check the component signature of an entity.
    void AddEntityToSystem(Entity entity);

    template <typename TComponent>
    TComponent &GetComponent(Entity entity) const;
};

template <typename TComponent>
void System::RequireComponent()
{
    const auto componentId = Component<TComponent>::GetId();
    componentSignature.set(componentId);
}

template <typename TSystem, typename... TArgs>
void Registry::AddSystem(TArgs &&...args)
{
    TSystem *newSystem = new TSystem(std::foorward<TArgs>(args)...);
    systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
}

template <typename TSystem>
void Registry::RemoveSystem()
{
    auto system = systems.find(std::type_index(typeid(TSystem)));
    if (system != systems.end())
    {
        delete system->second;
        systems.erase(system);
    }
}

template <typename TSystem>
bool Registry::HasSystem() const
{
    return systems.find(std::type_index(typeid(TSystem))) != systems.end();
}

template <typename TSystem>
TSystem &Registry::GetSystem() const
{
    auto system = systems.find(std::type_index(typeid(TSystem)));
    return *(std::static_pointer_cast<TSystem>(system->second));
}

template <typename TComponent, typename... TArgs>
void Registry::AddComponent(Entity entity, TArgs &&...args)
{
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();

    if (componentId >= componentPools.size())
    {
        componentPools.resize(componentId + 1, nullptr);
    }

    if (!componentPools[componentId])
    {
        Pool<TComponent> *newComponentPool = new Pool<TComponent>();
        componentPools[componentId] = newComponentPool;
    }

    Pool<TComponent> *componentPool = componentPools[componentId];

    if (entityId >= componentPool->GetSize())
    {
        componentPool->Resize(numEntities);
    }

    TComponent newComponent(std::forward<TArgs>(args)...);

    componentPool->Set(entityId, newComponent);

    entityComponentSignatures[entityId].set(componentId);
}

template <typename T>
void Registry::RemoveComponent(Entity entity)
{
    const auto componentId = Component<T>::GetId();
    const auto entityId = entity.GetId();

    entityComponentSignatures[entityId].set(componentId, false);
}

template <typename T>
bool Registry::HasComponent(Entity entity) const
{
    const auto componentId = Component<T>::GetId();
    const auto entityId = entity.GetId();

    return entityComponentSignatures[entityId].test(componentId);
}

#endif