#pragma once

#include <cstddef>
#include <stdint.h>
#include <vector>
#include <map>

#include "Component.h"
#include "ComponentPool.h"
#include "ChainPool.h"

class World {
public:
    typedef uint32_t entity_t;
    
    static const int MAX_COMPONENT_TYPES = 64;
    World();
    ~World() {
        for(auto ep : _entities) {
            destroyEntity(ep.first);
        }
    }
    
    entity_t createEntity() {
        entity_t entity = _entity_counter++;
        
        _entities[entity] = _chain_pool.create();
        
        return entity;
    }
    
    void destroyEntity(entity_t entity) {
        for(auto c : _chain_pool.all(_entities[entity])) {
            _component_pools[c.first].destroy(c.second);
        }
        _chain_pool.destroy(_entities[entity]);
    }
    
    template <typename C>
    ComponentPool::ComponentHandle createComponent(entity_t entity) {
        ComponentPool::ComponentHandle h = _component_pools[C::type()].template create<C>();
        _chain_pool.link(_entities[entity], C::type(), h);
        
        return h;
    }
    
    template <typename C>
    void destroyComponent(entity_t entity, ComponentPool::ComponentHandle component_handle) {
        _chain_pool.unlink(_entities[entity], component_handle);
        _component_pools[C::type()].destroy(component_handle);
    }
    
    template <typename C>
    C& resolve(ComponentPool::ComponentHandle handle) {
        return *_component_pools[C::type()].template resolve<C>(handle);
    }
    
    template <typename C>
    bool valid(ComponentPool::ComponentHandle handle) {
        return !_component_pools[C::type()].invalid(handle);
    }
private:
    std::vector<ComponentPool> _component_pools;
    ChainPool _chain_pool;
    
    std::map<entity_t, ChainPool::ChainHandle> _entities;
    
    entity_t _entity_counter;
};