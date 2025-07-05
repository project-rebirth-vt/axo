#include "api.h"

#include "mods/event.h"

static const luaL_Reg functions[] = {
  { NULL, NULL },
};

int luaopen_axo_event(lua_State* L) {
  luax_insistglobal(L, "axo");

  lua_newtable(L);
  luaL_setfuncs(L, functions, 0);
  lua_setfield(L, -2, "event");

  luax_assert(L, event_open());
  api_register_deinit(event_close);

  return 1;
}
