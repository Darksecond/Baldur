#include "System.h"

#include <list>

static std::list<System*> _systems;

void systems::registerSystem(System* system) {
    _systems.push_back(system);
}

void systems::step(int pass, double delta) {
    for(System* system : _systems) {
        system->step(pass, delta);
    }
}
