#include "TTLSystem.h"

#include <list>

#include "../Components/TTLComponent.h"

TTLSystem::TTLSystem(World* world) : System(world) {
    systems::registerSystem(this);
}

void TTLSystem::init() {
}

void TTLSystem::shutdown() {
}

bool TTLSystem::step(int pass, double delta) {
    if(pass != 1) return true;
    
    std::list<EntityHandle> to_delete;
    
    for(TTLComponent* component : world()->components<TTLComponent>()) {
        component->ttl -= delta;
        if(component->ttl <= 0) {
            to_delete.push_back(component->parent);
        }
    }
    
    for(EntityHandle handle : to_delete) {
        world()->destroyEntity(handle);
    }
    
    return true;
}

void TTLSystem::receive(const Event& event) {
}