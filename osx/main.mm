//#import <Foundation/Foundation.h>

#include <iostream>
#include <GLM/glm.hpp>
#include <GLEW/glew.h>
#include <GLFW/glfw.h>
#include <luajit-2.0/lua.hpp>

#include "World.h"

struct TestComponent : Component<TestComponent> {
    int a;
};

int main(int argc, char *argv[])
{
    World w;
    Entity* e = w.createEntity("Bob");
    TestComponent* tc = e->createComponent<TestComponent>();
    tc->a = 3;
    TestComponent* tc2 = e->createComponent<TestComponent>();
    tc2->a = 4;
    
    auto cs = e->components<TestComponent>();
    for(auto c : cs) {
        std::cout << c->a << std::endl;
    }
    
    e->destroyComponent<TestComponent>(tc);
    w.destroyEntity(e);
    return 0;
}
