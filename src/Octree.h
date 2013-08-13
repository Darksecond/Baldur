#pragma once

#include <GLM/glm.hpp>

#include <vector>
#include <list>
#include <iostream>

class Entity;
typedef Entity* EntityHandle;

class World;

typedef void (*traverser)(void* data, EntityHandle a, EntityHandle b);

class Octree {
public:
    Octree(World* world, const glm::vec3& center, double radius, unsigned level);
    ~Octree();
    
    void add(EntityHandle entity);
    void traverse(traverser, void* data);
    void traverse(traverser, void* data, EntityHandle a);
    
    inline void print() {
        std::cout << _level << ": " << _entities.size() << std::endl;
        
        for(Octree* child : _children) {
            if(child)
                child->print();
        }
    }
private:
    std::vector<Octree*> _children;
    std::list<EntityHandle> _entities;
    
    glm::vec3 _center;
    double _radius;
    unsigned _level;
    World* _world;
};