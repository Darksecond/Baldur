#pragma once

#include "System.h"

class TimeLeftSystem : public System {
public:
    TimeLeftSystem(World* world);
    virtual void init();
    virtual void shutdown();
    virtual bool step(int pass, double delta);
    virtual void receive(const Event& event);
private:
    EntityHandle _text_entity;
    float total_time;
};