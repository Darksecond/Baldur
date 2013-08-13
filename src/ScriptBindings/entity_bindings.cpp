#include "ScriptBindings.h"

#include "../World.h"
#include "../Entity.h"
#include "../Components/SpatialHierarchyComponent.h"
#include "../Components/SpatialComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Event.h"

static int script_entity_name(lua_State* L) {
    int n = lua_gettop(L);
    if(n != 2) return luaL_error(L, "Got %d arguments expected 2 (world, entity)", n);
    
    World* world = (World*)lua_touserdata(L, 1);
    EntityHandle entity = (EntityHandle)lua_touserdata(L, 2);
    
    const char* name = world->name(entity);
    
    lua_pushstring(L, name);
    return 1;
}

static int script_entity_set_local_position(lua_State* L) {
    int n = lua_gettop(L);
    if(n != 5) return luaL_error(L, "Got %d arguments expected 2 (world, entity, x, y, z)", n);
    
    World* world = (World*)lua_touserdata(L, 1);
    EntityHandle entity = (EntityHandle)lua_touserdata(L, 2);
    double x = luaL_checknumber(L, 3);
    double y = luaL_checknumber(L, 4);
    double z = luaL_checknumber(L, 5);
    
    SpatialHierarchyComponent* shc = world->component<SpatialHierarchyComponent>(entity);
    
    if(shc) {
        shc->local.translate(glm::vec3(x,y,z));
        lua_pushboolean(L, true);
    } else {
        lua_pushboolean(L, false);
    }
    
    return 1;
}

static int script_entity_set_position(lua_State* L) {
    int n = lua_gettop(L);
    if(n != 5) return luaL_error(L, "Got %d arguments expected 2 (world, entity, x, y, z)", n);
    
    World* world = (World*)lua_touserdata(L, 1);
    EntityHandle entity = (EntityHandle)lua_touserdata(L, 2);
    double x = luaL_checknumber(L, 3);
    double y = luaL_checknumber(L, 4);
    double z = luaL_checknumber(L, 5);
    
    SpatialComponent* sc = world->component<SpatialComponent>(entity);
    
    if(sc) {
        sc->spatial.translate(glm::vec3(x,y,z));
        events::sendEvent(Event{.type = EventType::MOVED, .entity = entity});
        lua_pushboolean(L, true);
    } else {
        lua_pushboolean(L, false);
    }
    
    return 1;
}

static int script_entity_local_lookat(lua_State* L) {
    int n = lua_gettop(L);
    if(n != 3) return luaL_error(L, "Got %d arguments expected 3 (world, entity, lookat_entity)", n);
    
    World* world = (World*)lua_touserdata(L, 1);
    EntityHandle entity = (EntityHandle)lua_touserdata(L, 2);
    EntityHandle lookat_entity = (EntityHandle)lua_touserdata(L, 3);
    
    SpatialHierarchyComponent* shc = world->component<SpatialHierarchyComponent>(entity);
    SpatialComponent* sc = world->component<SpatialComponent>(lookat_entity);
    if(shc && sc) {
        shc->local.look_at(sc->spatial);
        lua_pushboolean(L, true);
    } else {
        lua_pushboolean(L, false);
    }
    return 1;
}

static int get_mass(lua_State* L) {
    int n = lua_gettop(L);
    if(n != 2) return luaL_error(L, "Got %d arguments expected 2 (world, entity)", n);
    
    World* world = (World*)lua_touserdata(L, 1);
    EntityHandle entity = (EntityHandle)lua_touserdata(L, 2);
    
    RigidBodyComponent* component = world->component<RigidBodyComponent>(entity);
    if(component) {
        lua_pushnumber(L, component->mass);
    } else {
        lua_pushnil(L);
    }
    
    return 1;
}

static int get_position(lua_State* L) {
    int n = lua_gettop(L);
    if(n != 2) return luaL_error(L, "Got %d arguments expected 2 (world, entity)", n);
    
    World* world = (World*)lua_touserdata(L, 1);
    EntityHandle entity = (EntityHandle)lua_touserdata(L, 2);
    
    SpatialComponent* component = world->component<SpatialComponent>(entity);
    if(component) {
        lua_pushnumber(L, component->spatial.position().x);
        lua_pushnumber(L, component->spatial.position().y);
        lua_pushnumber(L, component->spatial.position().z);
    } else {
        lua_pushnil(L);
        lua_pushnil(L);
        lua_pushnil(L);
    }
    
    return 3;
}

static int set_mass(lua_State* L) {
    int n = lua_gettop(L);
    if(n != 3) return luaL_error(L, "Got %d arguments expected 3 (world, entity, mass)", n);
    
    World* world = (World*)lua_touserdata(L, 1);
    EntityHandle entity = (EntityHandle)lua_touserdata(L, 2);
    double new_mass = luaL_checknumber(L, 3);
    
    RigidBodyComponent* component = world->component<RigidBodyComponent>(entity);
    if(component) {
        component->mass = new_mass;
    }
    
    return 0;
}

static int get_velocity(lua_State* L) {
    int n = lua_gettop(L);
    if(n != 2) return luaL_error(L, "Got %d arguments expected 2 (world, entity)", n);
    
    World* world = (World*)lua_touserdata(L, 1);
    EntityHandle entity = (EntityHandle)lua_touserdata(L, 2);
    
    RigidBodyComponent* component = world->component<RigidBodyComponent>(entity);
    if(component) {
        lua_pushnumber(L, component->velocity.x);
        lua_pushnumber(L, component->velocity.y);
        lua_pushnumber(L, component->velocity.z);
    } else {
        lua_pushnil(L);
        lua_pushnil(L);
        lua_pushnil(L);
    }
    
    return 3;
}

static int set_velocity(lua_State* L) {
    int n = lua_gettop(L);
    if(n != 5) return luaL_error(L, "Got %d arguments expected 5 (world, entity, x, y, z)", n);
    
    World* world = (World*)lua_touserdata(L, 1);
    EntityHandle entity = (EntityHandle)lua_touserdata(L, 2);
    double x =  luaL_checknumber(L, 3);
    double y =  luaL_checknumber(L, 4);
    double z =  luaL_checknumber(L, 5);
    
    RigidBodyComponent* component = world->component<RigidBodyComponent>(entity);
    if(component) {
        component->velocity = glm::vec3(x, y, z);
    }
    
    return 0;
}

static int set_forward_velocity(lua_State* L) {
    int n = lua_gettop(L);
    if(n != 3) return luaL_error(L, "Got %d arguments expected 3 (world, entity, amount)", n);
    
    World* world = (World*)lua_touserdata(L, 1);
    EntityHandle entity = (EntityHandle)lua_touserdata(L, 2);
    double amount =  luaL_checknumber(L, 3);
    
    
    RigidBodyComponent* component = world->component<RigidBodyComponent>(entity);
    SpatialComponent* spatial = world->component<SpatialComponent>(entity);
    if(component && spatial) {
        component->velocity += spatial->spatial.forward() * (float)amount;
    }
    
    return 0;
}

void script_registerEntity(lua_State* L) {
    luaL_Reg regs[] = {
        {"name", script_entity_name},
        {"set_local_position", script_entity_set_local_position},
        {"local_lookat", script_entity_local_lookat},
        {"set_position", script_entity_set_position},
        {"get_position", get_position},
        {"get_mass", get_mass},
        {"set_mass", set_mass},
        {"get_velocity", get_velocity},
        {"set_velocity", set_velocity},
        {"set_forward_velocity", set_forward_velocity},
        {NULL, NULL},
    };
    luaL_register(L, "Entity", regs);
}
