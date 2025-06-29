#ifndef API_H
#define API_H

#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>

#define AXO_VERSION_MAJOR 0
#define AXO_VERSION_MINOR 1
#define AXO_VERSION_PATCH 0
#define AXO_VERSION_CODENAME "best friends"

int luaopen_axo(lua_State* L);
int luaopen_axo_boot(lua_State* L);

void luax_preload(lua_State* L, lua_CFunction f, const char* name);
void luax_insistglobal(lua_State* L, const char* k);

#endif
