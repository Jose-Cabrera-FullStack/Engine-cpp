#ifndef EVENTBUS_H
#define EVENTBUS_H

#include "../Logger/Logger.h"
#include "Event.h"
#include <map>
#include <typeindex>
#include <functional>
#include <list>

class IEventCallback
{

private:
    virtual void Call(Event &event) = 0;

public:
    virtual ~IEventCallback() = default;

    void Execute(Event &e)
    {
        Call(e);
    }
};

template <typename TOwner, typename TEvent>
class EventCallback : public IEventCallback
{
private:
    typedef void (TOwner::*CallbackFunction)(TEvent &);

    TOwner *ownerInstance;
    CallbackFunction callbackFunction;

    virtual void Call(Event &event) override
    {
        std::invoke(callbackFunction, ownerInstance, static_cast<TEvent &>(event));
    }

public:
    EventCallback(TOwner *ownerInstance, CallbackFunction callbackFunction)
    {
        this->ownerInstance = ownerInstance;
        this->callbackFunction = callbackFunction;
    }

    virtual ~EventCallback() = default;
};

typedef std::list<std::unique_ptr<IEventCallback>>
    HandlerList;

class EventBus
{
private:
    std::map<std::type_index, std::unique_ptr<HandlerList>> subscribers;

public:
    EventBus()
    {
        Logger::Log("EventBus created.");
    }

    ~EventBus()
    {
        Logger::Log("EventBus destructed.");
    }

    void Reset()
    {
        subscribers.clear();
    }

    //////////////////////////////////////////////////////////////////////////
    // Subscribe to an event type
    // In our implementation, a listener subscribes to an event type
    // Example: eventBust -> SubscribeToEvent<CollisionEvent>(this, &Game::OnCollision);
    //////////////////////////////////////////////////////////////////////////

    template <typename TEvent, typename TOwner>
    void SubscribeToEvent(TOwner *ownerInstance, void (TOwner::*callbackFunction)(TEvent &))
    {
        if (!subscribers[typeid(TEvent)].get())
        {
            subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
        }

        auto subscriber = std::make_unique<EventCallback<TOwner, TEvent>>(ownerInstance, callbackFunction);
        subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
    }

    //////////////////////////////////////////////////////////////////////////
    // Emit an event of type <T>
    // In our implementation, as soon as something emits an
    // event, we go ahead and execute all the listener callback function
    // Example: eventBust -> EmitEvent<CollisionEvent>(player, enemy);
    //////////////////////////////////////////////////////////////////////////

    template <typename TEvent, typename... TArgs>
    void EmitEvent(TArgs &&...args)
    {
        auto handlers = subscribers[typeid(TEvent)].get();
        if (handlers)
        {
            for (auto it = handlers->begin(); it != handlers->end(); it++)
            {
                auto handler = it->get();
                TEvent event(std::forward<TArgs>(args)...);
                handler->Execute(event);
            }
        }
    }
};

#endif