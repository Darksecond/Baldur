//#import <Foundation/Foundation.h>

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
    World::entity_t e = w.createEntity();
    auto h = w.createComponent<TestComponent>(e);
    w.resolve<TestComponent>(h).a = 3;
    w.destroyEntity(e);
    return 0;
}
