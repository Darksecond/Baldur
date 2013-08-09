#include "SpatialHierarchySystem.h"

#include "../Components/SpatialHierarchyComponent.h"
#include "../Components/SpatialComponent.h"

SpatialHierarchySystem::SpatialHierarchySystem(World* world) : System(world) {
    systems::registerSystem(this);
}

void SpatialHierarchySystem::init() {
}

void SpatialHierarchySystem::shutdown() {
}

bool SpatialHierarchySystem::step(int pass, double delta) {
    if(pass != 15) return true;
    
    for(SpatialHierarchyComponent* h : world()->components<SpatialHierarchyComponent>()) {
        SpatialComponent* spatial = world()->component<SpatialComponent>(h->parent);
        if(h && spatial) {
            SpatialComponent* parent_spatial = world()->component<SpatialComponent>(h->owner);
            if(parent_spatial) {
                //calculate new spatial
                spatial->spatial = parent_spatial->spatial * h->local;
                //spatial->spatial = h->local * parent_spatial->spatial;
            }
        }
    }
    
    return true;
}

void SpatialHierarchySystem::receive(const Event& event) {
}

void SpatialHierarchySystem::build() {
}
