#include "api.h"

static const luaL_Reg modules[] = {
  { "axo.boot", luaopen_axo_boot },
  { NULL, NULL },
};

int luaopen_axo(lua_State* L) {
  for (int i = 0; modules[i].name; i++) {
    luax_preload(L, modules[i].func, modules[i].name);
  }

  luax_insistglobal(L, "axo");

  return 1;
}

int luaopen_axo_boot(lua_State* L) {}
