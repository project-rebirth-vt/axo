#include "api.h"

#define MAX_DEINIT_FUNCS 16

static deinit_func deinit_funcs[MAX_DEINIT_FUNCS];
static int deinit_count = 0;

void api_register_deinit(deinit_func f) {
  if (deinit_count < MAX_DEINIT_FUNCS) {
    deinit_funcs[deinit_count++] = f;
  }
}

void api_cleanup(void) {
  for (int i = deinit_count - 1; i >= 0; --i) {
    if (deinit_funcs[i]) {
      deinit_funcs[i]();
    }
  }
  deinit_count = 0;
}

void luax_preload(lua_State* L, lua_CFunction f, const char* name) {
  lua_getglobal(L, "package");
  lua_getfield(L, -1, "preload");
  lua_pushcfunction(L, f);
  lua_setfield(L, -2, name);
  lua_pop(L, 2);
}

void luax_insistglobal(lua_State* L, const char* k) {
  lua_getglobal(L, k);
  if (!lua_istable(L, -1)) {
    lua_pop(L, 1);
    lua_newtable(L);
    lua_pushvalue(L, -1);
    lua_setglobal(L, k);
  }
}
