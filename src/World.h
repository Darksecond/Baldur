#pragma once

#include <cstddef>
#include <stdint.h>
#include <vector>
#include <map>

#include "Entity.h"
#include "Event.h"

class World {
public:
    ~World() {
        for(auto e : _entities) {
            if(e.second) delete e.second;
        }
    }
    
    Entity* createEntity(const char* name) {
        Entity* e = new Entity(name);
        _entities[name] = e;
        
        Event event = {
            .type = EventType::ENTITY_CREATED,
            .entity = e,
        };
        events::sendEvent(event);
        
        return e;
    }
    
    void destroyEntity(Entity* entity) {
        
        Event event = {
            .type = EventType::ENTITY_DESTROYED,
            .entity = entity,
        };
        events::sendEvent(event, true);
        
        _entities[entity->name()] = nullptr;
        delete entity;
    }
    
    Entity* resolve(const char* name) {
        return _entities[name];
    }
    
    template <typename C>
    C* createComponent(Entity* entity) {
        C* component = entity->createComponent<C>();
        _components[C::type()].push_back(component);
        return component;
    }
    
    template <typename C>
    void destroyComponent(Entity* entity, C* component) {
        _components[C::type()].remove(component);
        entity->destroyComponent(component);
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
    std::list<C*> components(Entity* entity) {
        return entity->components<C>();
    }
    
private:
    std::map<const char*, Entity*> _entities;
    std::map<BaseComponent::type_t, std::list<BaseComponent*>> _components;
};