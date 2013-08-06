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

private:
    std::map<const char*, Entity*> _entities;
};