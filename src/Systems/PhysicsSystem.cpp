#include "PhysicsSystem.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpatialComponent.h"

PhysicsSystem::PhysicsSystem(World* world) : System(world) {
    systems::registerSystem(this);
}

void PhysicsSystem::init() {
}

void PhysicsSystem::shutdown() {
}

bool PhysicsSystem::step(int pass, double delta) {
    if(pass != 12) return true;
    
    for(RigidBodyComponent* body : world()->components<RigidBodyComponent>()) {
        SpatialComponent* spatial = world()->component<SpatialComponent>(body->parent);
        
        spatial->spatial.translate(body->velocity);
        events::sendEvent(Event{.type = EventType::MOVED, .entity = body->parent});
    }
    
    return true;
}

void PhysicsSystem::receive(const Event& event) {
}

void PhysicsSystem::build() {
}