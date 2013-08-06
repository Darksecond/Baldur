#pragma once

#include <type_traits>

class BaseComponent {
public:
    typedef unsigned long type_t;
protected:
    static type_t _type_counter;
};

template <typename Derived>
class Component : public BaseComponent {
public:
    static const type_t type() {
        static type_t _type = _type_counter++;
        return _type;
    }
};