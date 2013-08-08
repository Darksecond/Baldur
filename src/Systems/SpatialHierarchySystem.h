#pragma once

#include "System.h"

class SpatialHierarchySystem : public System {
public:
    SpatialHierarchySystem(World* world);
    virtual void init();
    virtual void shutdown();
    virtual bool step(int pass, double delta);
    virtual void receive(const Event& event);
private:
};