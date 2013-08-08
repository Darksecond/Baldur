#pragma once

#include <type_traits>

class Entity;
typedef Entity* EntityHandle;

struct BaseComponent {
public:
    typedef unsigned long type_t;
    EntityHandle parent;
protected:
    static type_t _type_counter;
};

template <typename Derived>
struct Component : public BaseComponent {
public:
    static const type_t type() {
        static type_t _type = _type_counter++;
        return _type;
    }
};