#pragma once

#include "InputSystem.h"
#include "World.h"

class Game {
public:
    Game();
    void init();
    void shutdown();
    void run();
private:
    void initGLFW();
    void initGLEW();
    
    World _world;
    InputSystem _input_system;
};