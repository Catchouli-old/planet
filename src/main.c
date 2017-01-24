#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

void init_sdl(SDL_Window** window, SDL_GLContext* context);
void destroy_sdl(SDL_Window* window, SDL_GLContext context);

int main(int argc, char** argv) {
  SDL_Window* window;
  SDL_GLContext context;

  init_sdl(&window, &context);

  glClearColor(0,0,0,1);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(window);

  int run = 1;
  while(run) {
    SDL_Event evt;
    while (SDL_PollEvent(&evt)) {
      if (evt.type == SDL_WINDOWEVENT) {
        if (evt.window.event == SDL_WINDOWEVENT_CLOSE) {
          run = 0;
        }
      }
    }
  }

  destroy_sdl(window, context);

  return 0;
}

void init_sdl(SDL_Window** window, SDL_GLContext* context) {
  *window = SDL_CreateWindow("", 0, 0, 800, 600, SDL_WINDOW_OPENGL);
  *context = SDL_GL_CreateContext(*window);
}

void destroy_sdl(SDL_Window* window, SDL_GLContext context) {
  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
}

