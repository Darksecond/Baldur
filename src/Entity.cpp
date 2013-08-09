#include "Entity.h"

Entity::Entity(const char* name) : _name(name){
}

Entity::~Entity() {
    for(auto& c : _components) {
        for(auto& cc : c.second) {
            delete cc;
        }
    }
}

const char* Entity::name() const {
    return _name.c_str();
}
