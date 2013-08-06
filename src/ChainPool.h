#pragma once

#include <cstddef>
#include <stdint.h>
#include <vector>
#include <cassert>
#include <list>

#include "ComponentPool.h"

//TODO move to cpp file
class ChainPool {
public:
    struct ChainHandle {
        uint16_t generation;
        uint16_t index;
    };
    
    static const int DEFAULT_MAX_CHAINS = 32;
    
    ChainPool(const size_t max = DEFAULT_MAX_CHAINS) : _max(max), _generations(max), _used(max), _chains(max) {
    }
    
    ChainHandle create() {
        for(uint16_t i = 0; i < _max; ++i) {
            if(_used[i] == false) {
                _used[i] = true;
                return ChainHandle{_generations[i],i};
            }
        }
        return ChainHandle{static_cast<uint16_t>(-1), static_cast<uint16_t>(_max-1)};
    }
    
    void destroy(ChainHandle handle) {
        assert(!invalid(handle));
        
        _used[handle.index] = false;
        _chains[handle.index].clear();
    }
    
    void link(ChainHandle handle, BaseComponent::type_t type, ComponentPool::ComponentHandle component_handle) {
        assert(!invalid(handle));
        
        _chains[handle.index].push_back({type, component_handle});
    }
    
    void unlink(const ChainHandle handle, const ComponentPool::ComponentHandle component_handle) {
        assert(!invalid(handle));
        
        for(auto it = _chains[handle.index].begin(); it != _chains[handle.index].end(); ++it) {
            if(it->second.index == component_handle.index && it->second.generation == component_handle.generation) {
                _chains[handle.index].erase(it);
                return;
            }
        }
    }
    
    bool invalid(const ChainHandle handle) {
        if(_generations[handle.index] == handle.generation) {
            return false;
        } else {
            return true;
        }
    }
    
    std::list<std::pair<BaseComponent::type_t, ComponentPool::ComponentHandle>>& all(const ChainHandle handle) {
        assert(!invalid(handle));
        
        return _chains[handle.index];
    }
    //first
private:
    const size_t _max;
    
    std::vector<uint16_t> _generations;
    std::vector<bool> _used;
    std::vector<std::list<std::pair<BaseComponent::type_t, ComponentPool::ComponentHandle>>> _chains;
};