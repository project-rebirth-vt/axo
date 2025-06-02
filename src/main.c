#define GLAD_GLES2_IMPLEMENTATION
#include <glad/gles2.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("failed to initialize sdl: %s", SDL_GetError());
    return 1;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

  SDL_Window* window = SDL_CreateWindow("axo", 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  if (!window) {
    SDL_Log("failed to create window: %s", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  SDL_GLContext context = SDL_GL_CreateContext(window);
  if (!context) {
    SDL_Log("failed to create gl context: %s", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  int version = gladLoadGLES2((GLADloadfunc)SDL_GL_GetProcAddress);
  if (!version) {
    SDL_Log("failed to load gl es");
    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  SDL_Log("gl es vendor: %s", glGetString(GL_VENDOR));
  SDL_Log("gl es renderer: %s", glGetString(GL_RENDERER));
  SDL_Log("gl es version: %s", glGetString(GL_VERSION));

  if (!SDL_GL_SetSwapInterval(1)) {
    SDL_Log("failed to set vsync: %s", SDL_GetError());
  } else {
    SDL_Log("vsync enabled");
  }

  bool running = true;
  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        running = false;
      }
    }

    glClearColor(0.0f, 105.0f / 255.0f, 148.0f / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    SDL_GL_SwapWindow(window);
    SDL_Delay(1);
  }

  SDL_GL_DestroyContext(context);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
