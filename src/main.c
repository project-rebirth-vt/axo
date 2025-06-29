#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "api/api.h"

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv) {
  if (argc > 1 && SDL_strcmp(argv[1], "--version") == 0) {
    SDL_Log("axo engine: %d.%d.%d %s", AXO_VERSION_MAJOR, AXO_VERSION_MINOR, AXO_VERSION_PATCH, AXO_VERSION_CODENAME);
    return SDL_APP_SUCCESS;
  }

  if (argc > 1 && SDL_strcmp(argv[1], "--help") == 0) {
    SDL_Log(
        "tiny lua framework ≽(• O •)≼\n"
        "https://axoengine.org\n"
        "\n"
        "usage:\n"
        "  axo --version                  prints axo version and quits\n"
        "  axo --help                     prints this message and quits\n"
        "  axo path/to/gamedir            runs the game from the given directory which contains a main.lua file\n");
    return SDL_APP_SUCCESS;
  }

  lua_State* L = luaL_newstate();
  if (L == NULL) {
    SDL_Log("failed to create lua state");
    return SDL_APP_FAILURE;
  }

  *appstate = L;

  luaL_openlibs(L);
  luax_preload(L, luaopen_axo, "axo");

  lua_newtable(L);

  if (argc > 0) {
    lua_pushstring(L, argv[0]);
    lua_rawseti(L, -2, -2);
  }

  lua_pushstring(L, "embedded boot.lua");
  lua_rawseti(L, -2, -1);

  for (int i = 1; i < argc; i++) {
    lua_pushstring(L, argv[i]);
    lua_rawseti(L, -2, i);
  }

  lua_setglobal(L, "arg");

  lua_getglobal(L, "require");
  lua_pushstring(L, "axo");
  lua_call(L, 1, 1);
  lua_pop(L, 1);

  lua_getglobal(L, "require");
  lua_pushstring(L, "axo.boot");
  lua_call(L, 1, 1);

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
  (void)appstate;
  (void)event;
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
  lua_State* L = (lua_State*)appstate;

  int nres;
  if (lua_resume(L, NULL, 0, &nres) == LUA_YIELD) {
    lua_pop(L, nres);
    return SDL_APP_CONTINUE;
  } else {
    int retval = 0;
    if (lua_isnumber(L, -1)) {
      retval = lua_tointeger(L, -1);
    }

    if (retval != 0) {
      return SDL_APP_FAILURE;
    } else {
      return SDL_APP_SUCCESS;
    }
  }
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
  (void)result;

  lua_State* L = (lua_State*)appstate;
  if (L) {
    lua_close(L);
  }
}
