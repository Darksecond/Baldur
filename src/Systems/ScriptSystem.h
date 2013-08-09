#pragma once

#include <luajit-2.0/lua.hpp>

#include "System.h"

class ScriptSystem : public System {
public:
    ScriptSystem(World* world);
    virtual void init();
    virtual void shutdown();
    virtual bool step(int pass, double delta);
    virtual void receive(const Event& event);
    virtual void build();
private:
    lua_State* _L;
    int load(const char* identifier);
};