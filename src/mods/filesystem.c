#include "filesystem.h"

#include <SDL3/SDL.h>
#include <physfs.h>

bool filesystem_open(void) {
  SDL_Log("filesystem module initialized.");
  return true;
}

void filesystem_close(void) {
  SDL_Log("filesystem module closed.");
}

bool filesystem_init(const char* arg0) {
  if (!PHYSFS_init(arg0)) {
    return SDL_SetError("failed to initialize filesystem: %s", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
  }
  return true;
}

// in this case the bool return type does not indicate we need to exit with error
bool filesystem_set_identity(const char* identity, bool append_to_path) {
  if (!PHYSFS_isInit())
    return false;

  if (identity == NULL || SDL_strlen(identity) == 0) {
    return false;
  }

  const char* save_path = PHYSFS_getPrefDir("axo", identity);
  SDL_Log("%s", save_path);
}
