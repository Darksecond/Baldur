#pragma once

#include "InputSystem.h"
#include "RenderSystem.h"
#include "MovementControlSystem.h"
#include "SpatialHierarchySystem.h"
#include "TimeLeftSystem.h"
#include "TTLSystem.h"
#include "ScriptSystem.h"
#include "PhysicsSystem.h"

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
    MovementControlSystem _movement_control_system;
    SpatialHierarchySystem _spatial_hierarchy_system;
    TimeLeftSystem _time_left_system;
    TTLSystem _ttl_system;
    ScriptSystem _script_sytem;
    PhysicsSystem _physics_system;
};