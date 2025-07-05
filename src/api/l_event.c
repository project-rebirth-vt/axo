#include "api.h"

#include "mods/event.h"

static int l_event_pump(lua_State* L) {
  (void)L;
  event_pump();
  return 0;
}

static int l_event_poll(lua_State* L) {
  Event event;
  if (event_poll(&event)) {
    switch (event.type) {
      case EVENT_QUIT:
        lua_pushstring(L, "quit");
        return 1;
      default:
        lua_pushnil(L);
        return 1;
    }
  }
  return 0;
}

static const luaL_Reg functions[] = {
  { "pump", l_event_pump },
  { "poll", l_event_poll },
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
