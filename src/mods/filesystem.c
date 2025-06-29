#include "filesystem.h"

#include <SDL3/SDL.h>

bool filesystem_open(void) {
  SDL_Log("filesystem module initialized.");
  return true;
}

void filesystem_close(void) {
  SDL_Log("filesystem module closed.");
}
