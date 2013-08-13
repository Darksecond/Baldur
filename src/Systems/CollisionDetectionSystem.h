#pragma once

#include "../System.h"
#include "../Components/SphereGeomComponent.h"

class CollisionDetectionSystem : public System {
public:
    CollisionDetectionSystem(World* world);
    virtual void init();
    virtual void shutdown();
    virtual bool step(int pass, double delta);
    virtual void receive(const Event& event);
    virtual void build();
private:
    void sphere_sphere_collision(SphereGeomComponent* a, SphereGeomComponent* b);
    static void collider(void* data, EntityHandle a, EntityHandle b);
};