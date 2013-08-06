#pragma once

#include <vector>
#include <cstddef>
#include <stdint.h>
#include <cassert>

#include "Component.h"

class ComponentPool {
public:
    struct ComponentHandle {
        uint16_t generation;
        uint16_t index;
    };
    
    static const int DEFAULT_MAX_COMPONENTS = 1024;
    
    ComponentPool(const size_t max = DEFAULT_MAX_COMPONENTS);
    ~ComponentPool();
    
    void destroy(const ComponentHandle handle);
    bool invalid(const ComponentHandle handle);
    
    template <typename C>
    ComponentHandle create() {
        C* component = new C();
        for(uint16_t i = 0; i < _max; ++i) {
            if(_components[i] == nullptr) {
                _components[i] = component;
                return ComponentHandle{_generations[i], i};
            }
        }
        
        return ComponentHandle{static_cast<uint16_t>(-1), static_cast<uint16_t>(_max-1)};
    }
    
    template <typename C>
    C* resolve(const ComponentHandle handle) {
        if(invalid(handle)) {
            return nullptr;
        } else {
            return static_cast<C*>(_components[handle.index]);
        }
    }
    
    //TODO all
private:
    std::vector<BaseComponent*> _components;
    std::vector<uint16_t> _generations;
    const size_t _max;
};