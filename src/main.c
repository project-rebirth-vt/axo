#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "api/api.h"

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv) {
  if (argc > 1 && SDL_strcmp(argv[1], "--version") == 0) {
    SDL_Log("axo engine: %d.%d.%d", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_MICRO_VERSION);
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
  (void)appstate;
  return SDL_APP_SUCCESS;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
  (void)appstate;
  (void)result;
}
