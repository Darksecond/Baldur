#pragma once

#include "System.h"

class InputSystem : public System {
public:
    InputSystem(World* world);
    virtual void init();
    virtual void shutdown();
    virtual bool step(int pass, double delta);
    virtual void receive(const Event& event);
private:
    int mouse_x, mouse_y;
};