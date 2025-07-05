#include "event.h"

#include <SDL3/SDL.h>

bool event_open(void) {
  SDL_Log("event module initialized.");
  return true;
}

void event_close(void) {
  SDL_Log("event module closed.");
}
