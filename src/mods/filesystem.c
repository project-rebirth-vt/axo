#include "filesystem.h"

#include <SDL3/SDL.h>
#include <physfs.h>

static struct {
  const char* save_path_full;
} state;

bool filesystem_open(void) {
  SDL_Log("filesystem module initialized.");
  state.save_path_full = NULL;
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
  const char* old_save_path = NULL;

  if (state.save_path_full) {
    old_save_path = SDL_strdup(state.save_path_full);
  }

  state.save_path_full = PHYSFS_getPrefDir("axo", identity);

  if (old_save_path) {
    PHYSFS_unmount(old_save_path);
  }

  PHYSFS_mount(state.save_path_full, NULL, append_to_path);
  PHYSFS_setWriteDir(NULL);

  return true;
}

bool filesystem_set_source(const char* source) {
  if (!PHYSFS_isInit()) {
    return false;
  }

  if (source == NULL || SDL_strlen(source) == 0) {
    return false;
  }

  if (!PHYSFS_mount(source, NULL, true)) {
    return SDL_SetError("failed to mount source: %s", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
  }

  return true;
}
