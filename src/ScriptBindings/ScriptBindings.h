#pragma once

#include <luajit-2.0/lua.hpp>

void script_registerWorld(lua_State* L);
void script_registerEntity(lua_State* L);
void script_registerVector(lua_State* L);
