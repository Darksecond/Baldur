#include "System.h"

#include <list>

static std::list<System*> _systems;

void systems::registerSystem(System* system) {
    _systems.push_back(system);
}

bool systems::step(int pass, double delta) {
    bool keep_running = true;
    for(System* system : _systems) {
        if(system->step(pass, delta) == false) keep_running = false;
    }
    return keep_running;
}

void systems::build() {
    for(System* system : _systems) {
        system->build();
    }
}