#include "World.h"
#include "EntityParser.h"

World::World() {
}

World::~World() {
    for(auto& e: _entities) {
        delete e;
    }
}

EntityHandle World::createEntity(const char* name) {
    Entity* e = new Entity(name);
    _entities.push_back(e);
    
    Event event = {
        .type = EventType::ENTITY_CREATED,
        .entity = e,
    };
    events::sendEvent(event, true);
    
    return e;
}

void World::destroyEntity(const EntityHandle entity) {
    
    Event event = {
        .type = EventType::ENTITY_DESTROYED,
        .entity = entity,
    };
    events::sendEvent(event, true);
    
    for(auto& pair : _components) {
        pair.second.remove_if(
                              [&](const BaseComponent* value) {
                                  return value->parent == entity;
                              });
    }
    
    _entities.remove(entity);
    delete entity;
}

std::list<EntityHandle> World::entities() {
    return _entities;
}

const char* World::name(const EntityHandle entity) const {
    return entity->name();
}

EntityHandle World::loadEntity(const char* identifier, const char* name) {
    EntityParser parser;
    return parser.parse(this, identifier, name);
}
