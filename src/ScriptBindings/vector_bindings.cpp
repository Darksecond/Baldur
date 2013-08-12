#include "ScriptBindings.h"
#include <GLM/glm.hpp>
/*
 WARNING: this code is currently unused!
 https://gist.github.com/kizzx2/1594905
 http://cfc.kizzx2.com/index.php/binding-c-classes-to-lua-a-step-by-step-example-for-beginners/
 http://loadcode.blogspot.nl/2007/02/wrapping-c-classes-in-lua.html
 */

static int constructor(lua_State* L) {
    int n = lua_gettop(L);
    
    double x = 0;
    double y = 0;
    double z = 0;
    
    if(n >= 1)
        x = luaL_checknumber(L, 1);
    if(n >= 2)
        y = luaL_checknumber(L, 2);
    if(n >= 3)
        z = luaL_checknumber(L, 3);
    if(n >= 4)
        return luaL_error(L, "Got %d arguments expected 1-3 ([x], [y], [z])", n);
    
    glm::vec3** udata = (glm::vec3**)lua_newuserdata(L, sizeof(glm::vec3*));
    *udata = new glm::vec3(x,y,z);
    
    luaL_getmetatable(L, "luaL_Vector");
    lua_setmetatable(L, -2);
    
    return 1;
}

void script_registerVector(lua_State* L) {
    luaL_Reg regs[] = {
        {"new", constructor},
        {NULL, NULL},
    };
    
    luaL_newmetatable(L, "LuaL_Vector");
    luaL_register(L, NULL, regs);
    
    lua_pushvalue(L, -1);
    lua_setfield(L, -1, "__index");
    
    lua_setglobal(L, "Vector");
}
