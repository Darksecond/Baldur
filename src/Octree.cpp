#include "Octree.h"

#include "Entity.h"
#include "World.h"
#include "Components/SpatialComponent.h"
#include "Components/SphereGeomComponent.h"

Octree::Octree(World* world, const glm::vec3& center, double radius, unsigned level) : _children(8), _center(center), _radius(radius), _level(level), _world(world) {
}

Octree::~Octree() {
    for(Octree* child : _children) {
        if(child)
            delete child;
    }
}

void Octree::add(EntityHandle entity) {
    //TODO support more geoms
    if(_world->component<SpatialComponent>(entity) == nullptr || _world->component<SphereGeomComponent>(entity) == nullptr) return;
    
    
    if(_level == 0) {
        _entities.push_back(entity);
        return;
    }
    
    glm::vec3 position = _world->component<SpatialComponent>(entity)->spatial.position();
    double radius = _world->component<SphereGeomComponent>(entity)->radius;
    
    unsigned min_index =
      1*(position.x - radius >= _center.x)
    + 2*(position.y - radius >= _center.y)
    + 4*(position.z - radius >= _center.z);
    
    unsigned max_index =
      1*(position.x + radius >= _center.x)
    + 2*(position.y + radius >= _center.y)
    + 4*(position.z + radius >= _center.z);
    
    /*
    std::cout << "-------" << std::endl;
    std::cout << "min: " << min_index << std::endl;
    std::cout << "max: " << max_index << std::endl;
    std::cout << "center: " << _center.x << ", " << _center.y << ", " << _center.z << std::endl;
    std::cout << "position: " << position.x << ", " << position.y << ", " << position.z << std::endl;
    std::cout << "radius: " << radius << std::endl;
     */
    
    if(min_index == max_index) {
        if(!_children[min_index]) {
            glm::vec3 new_center(
            _center.x + _radius * ((min_index & 1) - 0.5f),
            _center.y + _radius * ((min_index & 2)/2 - 0.5f),
            _center.z + _radius * ((min_index & 4)/4 - 0.5f));
            _children[min_index] = new Octree(_world, new_center, _radius/2, _level-1);
        }
        _children[min_index]->add(entity);
    } else {
        _entities.push_back(entity);
    }
    
}

void Octree::traverse(traverser t, void* data) {
    for(auto it = _entities.begin(); it != _entities.end(); ++it) {
        auto inner_it = it;
        ++inner_it;
        for(; inner_it != _entities.end(); ++inner_it) {
            t(data, *it, *inner_it);
        }
        
        for(Octree* child : _children) {
            if(child)
                child->traverse(t, data, *it);
        }
    }
    
    for(Octree* child : _children) {
        if(child)
            child->traverse(t, data);
    }
}

void Octree::traverse(traverser t, void* data, EntityHandle a) {
    for(auto it = _entities.begin(); it != _entities.end(); ++it) {
        if(a != *it) {
            t(data, a, *it);
        }
    }
    
    for(Octree* child : _children) {
        if(child)
            child->traverse(t, data, a);
    }
}
