#pragma once

#include "World.h"
#include "Entity.h"
#include "Event.h"

class System : public Sink {
public:
    System(World* world) : _world(world) {}
    virtual ~System() = default;
    
    World* world() { return _world; }
    
    virtual void init() = 0;
    virtual void shutdown() = 0;
    virtual bool step(int pass, double delta) = 0;
private:
    World* _world;
};

namespace systems {
    void registerSystem(System* system);
    bool step(int pass, double delta);
}
