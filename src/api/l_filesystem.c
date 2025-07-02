#include "api.h"

#include "mods/filesystem.h"

static int l_filesystem_init(lua_State* L) {
  const char* arg0 = luaL_checkstring(L, 1);
  luax_assert(L, filesystem_init(arg0));
  return 0;
}

static int l_filesystem_set_identity(lua_State* L) {
  const char* identity = luaL_checkstring(L, 1);
  bool append_to_path = luax_optboolean(L, 2, false);
  lua_pushboolean(L, filesystem_set_identity(identity, append_to_path));
  return 1;
}

static const luaL_Reg functions[] = {
  { "init", l_filesystem_init },
  { "set_identity", l_filesystem_set_identity },
  { NULL, NULL },
};

int luaopen_axo_filesystem(lua_State* L) {
  luax_insistglobal(L, "axo");

  lua_newtable(L);
  luaL_setfuncs(L, functions, 0);
  lua_setfield(L, -2, "filesystem");

  luax_assert(L, filesystem_open());
  api_register_deinit(filesystem_close);

  return 1;
}
