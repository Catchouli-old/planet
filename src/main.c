#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "window.h"
#include "mesh.c"

int main(int argc, char** argv) {
  struct Window* window = window_new("", 800, 600);

  struct GpuMesh* mesh = gpu_mesh_load_obj("data/sphere.obj");

  int run = 1;
  while(run) {
    SDL_Event* evt;
    while ((evt = window_peek(window))) {
      if (evt->type == SDL_WINDOWEVENT) {
        if (evt->window.event == SDL_WINDOWEVENT_CLOSE) {
          run = 0;
        }
      }
    }

    // Render
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
    window_swap(window);
  }

  window_delete(window);

  return 0;
}

