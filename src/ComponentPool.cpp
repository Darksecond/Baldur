#include "ComponentPool.h"

ComponentPool::ComponentPool(const size_t max) : _components(max), _generations(max), _max(max) {}

ComponentPool::~ComponentPool() {
    for(BaseComponent* c : _components) {
        if(c) delete c;
    }
}

void ComponentPool::destroy(const ComponentHandle handle) {
    assert(!invalid(handle));
    
    delete _components[handle.index];
    
    _generations[handle.index]++;
    _components[handle.index] = nullptr;
}

bool ComponentPool::invalid(const ComponentHandle handle) {
    if(_generations[handle.index] == handle.generation) {
        return false;
    } else {
        return true;
    }
}
