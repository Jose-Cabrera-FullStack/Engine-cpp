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

    void Update();

    Entity CreateEntity();

    void AddEntityToSystem(Entity entity);
};

template <typename TComponent>
void System::RequireComponent()
{
    const auto componentId = Component<TComponent>::GetId();
    componentSignature.set(componentId);
}

#endif