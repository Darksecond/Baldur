#include "PhysicsSystem.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpatialComponent.h"

PhysicsSystem::PhysicsSystem(World* world) : System(world) {
    systems::registerSystem(this);
    events::registerSink(this, EventType::COLLISION);
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
    if(event.type != EventType::COLLISION) return;
    
    RigidBodyComponent* rigid_a = world()->component<RigidBodyComponent>(event.collision.a);
    RigidBodyComponent* rigid_b = world()->component<RigidBodyComponent>(event.collision.b);
    if(rigid_a && rigid_b) {
        //bounce
        rigid_a->velocity = (rigid_a->velocity * (rigid_a->mass - rigid_b->mass) + (2 * rigid_b->mass * rigid_b->velocity)) / (rigid_a->mass + rigid_b->mass);
        rigid_b->velocity = (rigid_b->velocity * (rigid_b->mass - rigid_a->mass) + (2 * rigid_a->mass * rigid_a->velocity)) / (rigid_b->mass + rigid_a->mass);
    }
}

void PhysicsSystem::build() {
}