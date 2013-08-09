#pragma once

#include <cstddef>
#include <stdint.h>
#include <vector>
#include <map>
#include <iostream>
#include <string>

#include "Entity.h"
#include "Event.h"

typedef Entity* EntityHandle;

class World {
public:
    World();
    World(const World&) = delete;
    World& operator=(const World&) = delete;
    
    ~World();
    
    EntityHandle createEntity(const char* name);
    void destroyEntity(const EntityHandle entity);
    EntityHandle resolve(const char* name);
    std::list<EntityHandle> entities();
    const char* name(const EntityHandle entity) const;
    
    template <typename C>
    C* createComponent(EntityHandle entity) {
        C* component = entity->createComponent<C>();
        _components[C::type()].push_back(component);
        return component;
    }
    
    template <typename C>
    void destroyComponent(EntityHandle entity, C* component) {
        _components[C::type()].remove(static_cast<BaseComponent*>(component));
        entity->destroyComponent(component);
    }
    
    template <typename C>
    std::list<C*> components() {
        std::list<C*> cmps;
        for(BaseComponent* bc : _components[C::type()]) {
            cmps.push_back(static_cast<C*>(bc));
        }
        return cmps;
    }
    
    template <typename C>
    std::list<C*> components(EntityHandle entity) {
        return entity->components<C>();
    }
    
    template <typename C>
    C* component(EntityHandle entity) {
        return entity->component<C>();
    }
private:
    std::map<std::string, EntityHandle> _entities;
    std::map<BaseComponent::type_t, std::list<BaseComponent*>> _components;
};