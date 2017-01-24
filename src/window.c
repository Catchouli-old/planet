#include "window.h"

#include <SDL2/SDL.h>

struct Window {
  SDL_Window* window;
  SDL_GLContext context;
};

struct Window* window_new(const char* title, int w, int h) {
  struct Window* window = malloc(sizeof(struct Window));

  window->window = SDL_CreateWindow("", 0, 0, 800, 600, SDL_WINDOW_OPENGL);
  window->context = SDL_GL_CreateContext(window->window);

  return window;
}

void window_delete(struct Window* window) {
  SDL_GL_DeleteContext(window->context);
  SDL_DestroyWindow(window->window);

  free(window);
}

void window_swap(struct Window* window) {
  SDL_GL_SwapWindow(window->window);
}

union SDL_Event* window_peek() {
  static SDL_Event evt;
  if (SDL_PollEvent(&evt))
    return &evt;
  else
    return NULL;
}
