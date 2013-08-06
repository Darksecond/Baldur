#pragma once

#include <map>
#include <list>

#include "Component.h"

class Entity {
public:
    Entity(const char* name) : _name(name) {}
    
    ~Entity() {
        for(auto c : _components) {
            for(auto cc : c.second) {
                delete cc;
            }
        }
    }
    
    const char* name() { return _name; }
    
    template <typename C>
    C* createComponent() {
        C* new_c = new C();
        _components[C::type()].push_back(new_c);
        return new_c;
    }
    
    template <typename C>
    void destroyComponent(C* component) {
        _components[C::type()].remove(component);
    }
    
    template <typename C>
    std::list<C*> components() {
        std::list<C*> components;
        for(BaseComponent* bc : _components[C::type()]) {
            components.push_back(static_cast<C*>(bc));
        }
        return components;
    }
    
private:
    const char* _name;
    std::map<BaseComponent::type_t, std::list<BaseComponent*>> _components;
};