#include "CollisionDetectionSystem.h"
#include "../Components/SpatialComponent.h"
#include "../Octree.h"

CollisionDetectionSystem::CollisionDetectionSystem(World* world) : System(world) {
    systems::registerSystem(this);
    events::registerSink(this, EventType::MOVED);
}

void CollisionDetectionSystem::init() {
}

void CollisionDetectionSystem::shutdown() {
}

void CollisionDetectionSystem::collider(void* data, EntityHandle a, EntityHandle b) {
    CollisionDetectionSystem* system = (CollisionDetectionSystem*)data;
    
    system->sphere_sphere_collision(system->world()->component<SphereGeomComponent>(a), system->world()->component<SphereGeomComponent>(b));
}

bool CollisionDetectionSystem::step(int pass, double delta) {
    if(pass != 11) return true;
    
    Octree octree(world(), glm::vec3(-1), 100, 10);
    for(EntityHandle entity : world()->entities()) {
        octree.add(entity);
    }
    
    //octree.print();
    
    octree.traverse(collider, this);
    
    /*
    auto spheres = world()->components<SphereGeomComponent>();
    for(auto it = spheres.begin(); it != spheres.end(); ++it) {
        auto inner_it = it;
        ++inner_it;
        for(; inner_it != spheres.end(); ++inner_it) {
            if((*it)->parent == (*inner_it)->parent) continue;
            if(world()->component<SpatialComponent>((*it)->parent) == nullptr) continue;
            if(world()->component<SpatialComponent>((*inner_it)->parent) == nullptr) continue;
            sphere_sphere_collision(*it, *inner_it);
        }
    }
     */
    
    return true;
}

void CollisionDetectionSystem::receive(const Event& event) {
    switch (event.type) {
        case EventType::MOVED: {
        }
            break;
        default:
            break;
    };
}

void CollisionDetectionSystem::build() {
}

void CollisionDetectionSystem::sphere_sphere_collision(SphereGeomComponent* a, SphereGeomComponent* b) {
    SpatialComponent* spatial_a = world()->component<SpatialComponent>(a->parent);
    SpatialComponent* spatial_b = world()->component<SpatialComponent>(b->parent);
    
    glm::vec3 pos_a = spatial_a->spatial.position() + a->offset.position();
    glm::vec3 pos_b = spatial_b->spatial.position() + b->offset.position();
    
    glm::vec3 normal = pos_a - pos_b;
    float length = glm::length(normal);
    float radii = a->radius + b->radius;
    glm::vec3 point = glm::normalize(normal) * (float)a->radius;
    if(length < radii) {
        float depth = radii - length;
        normal = glm::normalize(normal);
        events::sendEvent(Event{.type = EventType::COLLISION,
            .collision = {
                .a = a->parent,
                .b = b->parent,
                .normal = {[0] = normal.x, [1] = normal.y, [2] = normal.z},
                .depth = depth,
                .position = {[0] = point.x, [1] = point.y, [2] = point.z},
            },
        });
    }
}
