#pragma once

#include "System.h"

class PhysicsSystem : public System {
public:
    PhysicsSystem(World* world);
    virtual void init();
    virtual void shutdown();
    virtual bool step(int pass, double delta);
    virtual void receive(const Event& event);
    virtual void build();
private:
};