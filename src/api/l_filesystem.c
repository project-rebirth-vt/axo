#include "api.h"

#include "mods/filesystem.h"

static const luaL_Reg functions[] = {
  { NULL, NULL },
};

int luaopen_axo_filesystem(lua_State* L) {
  luax_insistglobal(L, "axo");

  lua_newtable(L);
  luaL_setfuncs(L, functions, 0);
  lua_setfield(L, -2, "filesystem");

  luax_assert(L, filesystem_open());

  return 1;
}
