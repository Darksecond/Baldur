#pragma once

#include <map>
#include <list>

#include "Component.h"
#include "Event.h"

class Entity {
public:
    Entity(const char* name);
    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;
    
    ~Entity();
    
    const char* name() const;
    
    template <typename C>
    C* createComponent() {
        C* new_c = new C();
        new_c->parent = this;
        _components[C::type()].push_back(new_c);
        
        Event e = {
            .type = EventType::COMPONENT_CREATED,
            .entity = this,
        };
        events::sendEvent(e, true);
        
        return new_c;
    }
    
    template <typename C>
    void destroyComponent(C* component) {
        
        Event e = {
            .type = EventType::COMPONENT_DESTROYED,
            .entity = this,
        };
        events::sendEvent(e, true);
        
        _components[C::type()].remove(component);
        delete component;
    }
    
    template <typename C>
    std::list<C*> components() {
        std::list<C*> components;
        for(BaseComponent* bc : _components[C::type()]) {
            components.push_back(static_cast<C*>(bc));
        }
        return components;
    }
    
    template <typename C>
    C* component() {
        const auto& cs = components<C>();
        if(cs.empty())
            return nullptr;
        else
            return cs.front();
    }
    
private:
    const char* _name;
    std::map<BaseComponent::type_t, std::list<BaseComponent*>> _components;
};