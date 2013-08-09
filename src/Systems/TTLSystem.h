#pragma once

#include "System.h"

class TTLSystem : public System {
public:
    TTLSystem(World* world);
    virtual void init();
    virtual void shutdown();
    virtual bool step(int pass, double delta);
    virtual void receive(const Event& event);
    virtual void build();
private:
};