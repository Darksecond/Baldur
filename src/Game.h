#pragma once

#include "InputSystem.h"
#include "RenderSystem.h"
#include "World.h"

class Game {
public:
    Game();
    void init();
    void shutdown();
    void run();
    void build();
private:
    void initGLFW();
    void initGLEW();
    
    World _world;
    InputSystem _input_system;
    RenderSystem _render_system;
};