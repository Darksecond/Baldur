//#import <Foundation/Foundation.h>

#include <iostream>
#include <GLM/glm.hpp>
#include <GLEW/glew.h>
#include <GLFW/glfw.h>
#include <luajit-2.0/lua.hpp>

#include "World.h"
#include "System.h"

struct TestComponent : Component<TestComponent> {
    int a;
};

class TestSystem : public System {
public:
    TestSystem(World* w) : System(w) {}
    
    virtual void receive(const Event& event) {
        std::cout << "RECEIVE!" << std::endl;
    }
    
    virtual void step(int pass, double delta) {
        std::cout << "STEP!" << std::endl;
    }
private:
};

int main(int argc, char *argv[])
{
    World w;
    
    TestSystem sys(&w);
    systems::registerSystem(&sys);
    
    events::registerSink(&sys);
    
    events::sendEvent(Event{EventType::ALL});
    
    systems::step(0, 0);
    events::runEvents();
    
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
