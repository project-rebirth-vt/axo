#define GLAD_GLES2_IMPLEMENTATION
#include <glad/gles2.h>

#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static SDL_Window* window = NULL;
static SDL_GLContext context = NULL;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv) {
  (void)appstate;
  (void)argc;
  (void)argv;

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("failed to initialize sdl: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

  window = SDL_CreateWindow("axo", 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  if (!window) {
    SDL_Log("failed to create window: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  context = SDL_GL_CreateContext(window);
  if (!context) {
    SDL_Log("failed to create gl context: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  int version = gladLoadGLES2((GLADloadfunc)SDL_GL_GetProcAddress);
  if (!version) {
    SDL_Log("failed to load gl functions");
    return SDL_APP_FAILURE;
  }

  SDL_Log("gl vendor: %s", glGetString(GL_VENDOR));
  SDL_Log("gl renderer: %s", glGetString(GL_RENDERER));
  SDL_Log("gl version: %s", glGetString(GL_VERSION));

  SDL_GL_SetSwapInterval(1);

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
  (void)appstate;

  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  }

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
  (void)appstate;

  glClearColor(0.0f, 105.0f / 255.0f, 148.0f / 255.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  SDL_GL_SwapWindow(window);

  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
  (void)appstate;
  (void)result;

  if (context) {
    SDL_GL_DestroyContext(context);
  }

  if (window) {
    SDL_DestroyWindow(window);
  }
}
