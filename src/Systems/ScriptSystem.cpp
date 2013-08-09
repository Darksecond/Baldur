#include "ScriptSystem.h"

#include "../Resources/resource_factory.h"

ScriptSystem::ScriptSystem(World* world) : System(world) {
    systems::registerSystem(this);
}

static int script_world_createEntity(lua_State* L) {
    int n = lua_gettop(L);
    if(n != 3) return luaL_error(L, "Got %d arguments expected 3 (self, world, name)", n);
    
    World* world = (World*)lua_touserdata(L, 2);
    const char* name = luaL_checkstring(L, 3);
    EntityHandle entity = world->createEntity(name);
    
    lua_pushlightuserdata(L, entity);
    return 1;
}

static int script_world_destroyEntity(lua_State* L) {
    int n = lua_gettop(L);
    if(n != 3) return luaL_error(L, "Got %d arguments expected 3 (self, world, entity)", n);
    
    World* world = (World*)lua_touserdata(L, 2);
    EntityHandle entity = (EntityHandle)lua_touserdata(L, 3);
    
    world->destroyEntity(entity);
    
    return 0;
}

static void script_registerWorld(lua_State* L) {
    luaL_Reg regs[] = {
        {"createEntity", script_world_createEntity},
        {"destroyEntity", script_world_destroyEntity},
        {NULL,NULL},
    };
    luaL_register(L, "World", regs);
}

static int script_entity_name(lua_State* L) {
    int n = lua_gettop(L);
    if(n != 3) return luaL_error(L, "Got %d arguments expected 3 (self, world, entity)", n);
    
    World* world = (World*)lua_touserdata(L, 2);
    EntityHandle entity = (EntityHandle)lua_touserdata(L, 3);
    
    const char* name = world->name(entity);
    
    lua_pushstring(L, name);
    return 1;
}

//TODO change this to set_ttl and get_ttl?
#include "../Components/TTLComponent.h"
static int script_entity_createTTLComponent(lua_State* L) {
    int n = lua_gettop(L);
    if(n != 4) return luaL_error(L, "Got %d arguments expected 4 (self, world, entity, ttl_value)", n);
    
    World* world = (World*)lua_touserdata(L, 2);
    EntityHandle entity = (EntityHandle)lua_touserdata(L, 3);
    double ttl = luaL_checknumber(L, 4);
    
    TTLComponent* component = world->createComponent<TTLComponent>(entity);
    component->ttl = ttl;
    
    lua_pushlightuserdata(L, component);
    return 1;
}

static void script_registerEntity(lua_State* L) {
    luaL_Reg regs[] = {
        {"name", script_entity_name},
        {"createTTLComponent", script_entity_createTTLComponent},
        {NULL, NULL},
    };
    luaL_register(L, "Entity", regs);
}

void ScriptSystem::init() {
    _L = lua_open();
    
    luaopen_base(_L);
    luaopen_math(_L);
    luaopen_string(_L);
    luaopen_table(_L);
    luaopen_bit(_L);
    
    script_registerWorld(_L);
    script_registerEntity(_L);
    
    lua_pushlightuserdata(_L, world());
    lua_setglobal(_L, "world");
}

void ScriptSystem::build() {
    load("test.lua");
    load("test2.lua");
}

void ScriptSystem::shutdown() {
    lua_close(_L);
}

bool ScriptSystem::step(int pass, double delta) {
    if(pass != 8) return true;
    return true;
}

int ScriptSystem::load(const char* identifier) {
    auto reader = resource_factory::instance().manifests().read(identifier);
    size_t max_size = reader->size();
    char* buff = new char[max_size];
    size_t sz = reader->read(buff, max_size);
    int err = luaL_loadbuffer(_L, buff, sz, identifier);
    err = lua_pcall(_L, 0, LUA_MULTRET, 0);
    if(err != 0) {
        //an error has occured
        std::cerr << "--- ScriptSystem Error: " << std::endl;
        std::cerr << "------ " <<  lua_tostring(_L, -1) << std::endl;
        lua_pop(_L, 1);
    }
    return err;
}

void ScriptSystem::receive(const Event& event) {
}