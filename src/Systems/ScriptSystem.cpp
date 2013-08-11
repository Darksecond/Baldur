#include "ScriptSystem.h"

#include "../Resources/resource_factory.h"
#include "../ScriptBindings/ScriptBindings.h"

ScriptSystem::ScriptSystem(World* world) : System(world) {
    systems::registerSystem(this);
}

static int script_custom_loader(lua_State* L) {
    std::string module = lua_tostring(L, 1);
    module += ".lua";
    
    auto reader = resource_factory::instance().manifests().read(module);
    size_t max_size = reader->size();
    char* buff = new char[max_size];
    size_t sz = reader->read(buff, max_size);
    
    luaL_loadbuffer(L, buff, sz, module.c_str());
    return 1;
}

static void script_register_custom_loader(lua_State* L) {
    lua_getfield(L, LUA_GLOBALSINDEX, "package");
    lua_getfield(L, -1, "loaders");
    lua_remove(L, -2);
    
    int loader_amount = 0;
    lua_pushnil(L);
    while(lua_next(L, -2) != 0) {
        lua_pop(L, 1);
        ++loader_amount;
    }
    
    lua_pushinteger(L, loader_amount+1);
    lua_pushcfunction(L, script_custom_loader);
    lua_rawset(L,-3);
    lua_pop(L, 1);
}

void ScriptSystem::init() {
    _L = lua_open();
    
    luaL_openlibs(_L);
    
    script_register_custom_loader(_L);
    script_registerWorld(_L);
    script_registerEntity(_L);
    
    lua_pushlightuserdata(_L, world());
    lua_setglobal(_L, "world");
}

void ScriptSystem::build() {
    load("test.lua");
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