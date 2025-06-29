#include "api.h"

#include "boot.h"

static const luaL_Reg modules[] = {
  { "axo.boot", luaopen_axo_boot },
  { "axo.filesystem", luaopen_axo_filesystem },
  { NULL, NULL },
};

int luaopen_axo(lua_State* L) {
  for (int i = 0; modules[i].name; i++) {
    luax_preload(L, modules[i].func, modules[i].name);
  }

  luax_insistglobal(L, "axo");

  return 1;
}

int luaopen_axo_boot(lua_State* L) {
  if (luaL_loadbuffer(L, (const char*)boot, boot_len, "axo.boot") == LUA_OK) {
    lua_call(L, 0, 1);
  }
  return 1;
}
