#include "MovementControlSystem.h"

#include "../Components/ControlComponent.h"
#include "../Components/SpatialComponent.h"
#include "../Components/MouseComponent.h"

MovementControlSystem::MovementControlSystem(World* world) : System(world) {
    systems::registerSystem(this);
    events::registerSink(this, EventType::COMPONENT_CREATED);
    events::registerSink(this, EventType::KEY_PRESSED);
}

void MovementControlSystem::init() {
}

void MovementControlSystem::shutdown() {
}

bool MovementControlSystem::step(int pass, double delta) {
    if(pass != 2) return true;
    
    for(auto control : world()->components<ControlComponent>()) {
        auto mouse = world()->component<MouseComponent>(control->parent);
        auto spatial = world()->component<SpatialComponent>(control->parent);
        if(control && mouse && spatial) {
            //move entity
            
            float speed = 10;
            spatial->spatial.rotate(glm::vec3(1,0,0), delta * -mouse->y * speed); //pitch
            spatial->spatial.rotate(glm::vec3(0,1,0), delta * -mouse->x * speed); //yaw
            spatial->spatial.rotate(glm::vec3(0,0,1), delta * control->roll * speed); //roll
            
            spatial->spatial.translate(spatial->spatial.right() * (float)delta * control->movement.x);
            spatial->spatial.translate(spatial->spatial.up() * (float)delta * control->movement.y);
            spatial->spatial.translate(spatial->spatial.forward() * (float)delta * control->movement.z);
            
            
            control->movement = glm::vec3(0,0,0);
            control->roll = 0;
        }
    }
    
    return true;
}

void MovementControlSystem::receive(const Event& event) {
    if(event.type == EventType::COMPONENT_CREATED) {
        ControlComponent* control = world()->component<ControlComponent>(event.entity);
        MouseComponent* mouse = world()->component<MouseComponent>(event.entity);
        if(control != nullptr && mouse == nullptr) {
            world()->createComponent<MouseComponent>(event.entity);
        }
    } else if(event.type == EventType::KEY_PRESSED) {
        //TODO only for FREECAM
        for(auto control : world()->components<ControlComponent>()) {
            if(event.key == 'W') control->movement.z =  1;
            if(event.key == 'S') control->movement.z = -1;
            if(event.key == 'A') control->movement.x = -1;
            if(event.key == 'D') control->movement.x =  1;
            if(event.key == 'Z') control->movement.y =  1;
            if(event.key == 'X') control->movement.y = -1;
            if(event.key == 'Q') control->roll = -1;
            if(event.key == 'E') control->roll = 1;
        }
    }
}