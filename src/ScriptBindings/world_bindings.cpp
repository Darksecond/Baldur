#include "ScriptBindings.h"

#include "../World.h"
#include "../Entity.h"
#include "../Components/SpatialHierarchyComponent.h"

static int script_world_loadEntity(lua_State* L) {
    int n = lua_gettop(L);
    if(n != 3) return luaL_error(L, "Got %d arguments expected 3 (world, identifier, name)", n);
    
    World* world = (World*)lua_touserdata(L, 1);
    const char* identifier = luaL_checkstring(L, 2);
    const char* name = luaL_checkstring(L, 3);
    
    EntityHandle entity = world->loadEntity(identifier, name);
    
    lua_pushlightuserdata(L, entity);
    return 1;
}

static int script_world_destroyEntity(lua_State* L) {
    int n = lua_gettop(L);
    if(n != 2) return luaL_error(L, "Got %d arguments expected 2 (world, entity)", n);
    
    World* world = (World*)lua_touserdata(L, 1);
    EntityHandle entity = (EntityHandle)lua_touserdata(L, 2);
    
    world->destroyEntity(entity);
    
    return 0;
}

static int script_world_link(lua_State* L) {
    int n = lua_gettop(L);
    if(n != 3) return luaL_error(L, "Got %d arguments expected 3 (world, parent, child)", n);
    
    World* world = (World*)lua_touserdata(L, 1);
    EntityHandle parent = (EntityHandle)lua_touserdata(L, 2);
    EntityHandle child = (EntityHandle)lua_touserdata(L, 3);
    
    SpatialHierarchyComponent* shc = world->component<SpatialHierarchyComponent>(child);
    if(shc == nullptr) {
        shc = world->createComponent<SpatialHierarchyComponent>(child);
    }
    
    shc->owner = parent;
    
    return 0;
}

void script_registerWorld(lua_State* L) {
    luaL_Reg regs[] = {
        {"destroyEntity", script_world_destroyEntity},
        {"loadEntity", script_world_loadEntity},
        {"link", script_world_link},
        {NULL,NULL},
    };
    luaL_register(L, "World", regs);
}