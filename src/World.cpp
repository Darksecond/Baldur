#include "World.h"
#include "EntityParser.h"

World::World() {
}

World::~World() {
    for(auto& e : _entities) {
        if(e.second) delete e.second;
    }
}

EntityHandle World::createEntity(const char* name) {
    Entity* e = new Entity(name);
    _entities[e->name()] = e;
    
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
    
    _entities[entity->name()] = nullptr;
    delete entity;
}

EntityHandle World::resolve(const char* name) {
    return _entities[name];
}

std::list<EntityHandle> World::entities() {
    std::list<EntityHandle> entity_list;
    for(auto& pair : _entities) {
        if(pair.second != nullptr)
            entity_list.push_back(pair.second);
    }
    return entity_list;
}

const char* World::name(const EntityHandle entity) const {
    return entity->name();
}

EntityHandle World::loadEntity(const char* identifier, const char* name) {
    EntityParser parser;
    return parser.parse(this, identifier, name);
}
