#ifndef API_H
#define API_H

#include <SDL3/SDL.h>

#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>

#define AXO_VERSION_MAJOR 0
#define AXO_VERSION_MINOR 1
#define AXO_VERSION_PATCH 0
#define AXO_VERSION_CODENAME "best friends"

// cleanup logic
typedef void (*deinit_func)(void);

void api_register_deinit(deinit_func f);
void api_cleanup(void);

int luaopen_axo(lua_State* L);
int luaopen_axo_boot(lua_State* L);
int luaopen_axo_filesystem(lua_State* L);
int luaopen_axo_event(lua_State* L);

#define luax_assert(L, c)          \
  if (!(c)) {                      \
    luaL_error(L, SDL_GetError()); \
  }

void luax_preload(lua_State* L, lua_CFunction f, const char* name);
void luax_insistglobal(lua_State* L, const char* k);
bool luax_optboolean(lua_State* L, int idx, bool b);

#endif
