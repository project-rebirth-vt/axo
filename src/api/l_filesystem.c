#include "api.h"

#include "mods/filesystem.h"

#include <physfs.h>

static int loader(lua_State* L) {
  const char* module_name = luaL_checkstring(L, 1);

  luaL_Buffer b;
  luaL_buffinit(L, &b);
  for (const char* p = module_name; *p; p++) {
    luaL_addchar(&b, *p == '.' ? '/' : *p);
  }
  luaL_addstring(&b, ".lua");
  luaL_pushresult(&b);

  const char* filename = lua_tostring(L, -1);

  PHYSFS_File* file = PHYSFS_openRead(filename);
  if (!file) {
    lua_pushfstring(L, "cannot open file '%s': %s", filename, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
    return 1;
  }

  PHYSFS_sint64 size = PHYSFS_fileLength(file);
  char* buffer = (char*)SDL_malloc(size);
  if (!buffer) {
    PHYSFS_close(file);
    return luaL_error(L, "out of memory");
  }

  PHYSFS_readBytes(file, buffer, size);
  PHYSFS_close(file);

  if (luaL_loadbuffer(L, buffer, size, filename) != LUA_OK) {
    SDL_free(buffer);
    return lua_error(L);
  }

  SDL_free(buffer);
  return 1;
}

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

static int l_filesystem_set_source(lua_State* L) {
  const char* source = luaL_checkstring(L, 1);
  luax_assert(L, filesystem_set_source(source));
  return 0;
}

static const luaL_Reg functions[] = {
  { "init", l_filesystem_init },
  { "set_identity", l_filesystem_set_identity },
  { "set_source", l_filesystem_set_source },
  { NULL, NULL },
};

int luaopen_axo_filesystem(lua_State* L) {
  luax_insistglobal(L, "axo");

  lua_newtable(L);
  luaL_setfuncs(L, functions, 0);
  lua_setfield(L, -2, "filesystem");

  luax_assert(L, filesystem_open());
  api_register_deinit(filesystem_close);

  lua_getglobal(L, "package");
  lua_getfield(L, -1, "searchers");

  int n = lua_rawlen(L, -1);
  for (int i = n + 1; i > 1; i--) {
    lua_rawgeti(L, -1, i - 1);
    lua_rawseti(L, -2, i);
  }

  lua_pushcfunction(L, loader);
  lua_rawseti(L, -2, 1);

  lua_pop(L, 2);

  return 1;
}
