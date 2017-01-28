#include <stdio.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include "window.h"
#include "mesh.h"
#include "maths.h"

int main(int argc, char** argv) {
  // Create window
  struct Window* window = window_new("", 800, 600);

  // Initialise glew
  GLenum err = glewInit();
  if (err != GLEW_OK) {
    fprintf(stderr, "Failed to initialise glew: %s\n", glewGetErrorString(err));
  }

  // Load a mesh
  struct GpuMesh* mesh = gpu_mesh_load_obj("data/cube.obj");
  
  // Camera parameters
  float x = 0, y = 0, z = 0.2;
  float rx = 0, ry = 0;
  int mouse_down = 0;

  // Get initial mouse state
  int mouse_x, mouse_y;
  SDL_GetMouseState(&mouse_x, &mouse_y);

  // A vbo containing a triangle
  static const GLfloat g_vertex_buffer_data[] = {
     -1.0f, -1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
     0.0f,  1.0f, 0.0f,
  };
  GLuint vertexbuffer;
  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

  // Main loop
  int run = 1;
  while(run) {
    SDL_Event* evt;
    while ((evt = window_peek(window))) {
      if (evt->type == SDL_WINDOWEVENT) {
        if (evt->window.event == SDL_WINDOWEVENT_CLOSE) {
          run = 0;
        }
      }
      else if (evt->type == SDL_KEYDOWN) {
        if (evt->key.keysym.scancode == SDL_SCANCODE_W)
          z -= 0.1f;
        else if (evt->key.keysym.scancode == SDL_SCANCODE_S)
          z += 0.1f;
        else if (evt->key.keysym.scancode == SDL_SCANCODE_A)
          x -= 0.1f;
        else if (evt->key.keysym.scancode == SDL_SCANCODE_D)
          x += 0.1f;
      }
      else if (evt->type == SDL_MOUSEBUTTONDOWN) {
        SDL_SetRelativeMouseMode(1);
        mouse_down = 1;
      }
      else if (evt->type == SDL_MOUSEBUTTONUP) {
        SDL_SetRelativeMouseMode(0);
        mouse_down = 0;
      }
    }

    // Get difference in mouse position
    int new_mouse_x, new_mouse_y;
    SDL_GetMouseState(&new_mouse_x, &new_mouse_y);
    int mouse_dx = new_mouse_x - mouse_x;
    int mouse_dy = new_mouse_y - mouse_y;
    mouse_x = new_mouse_x;
    mouse_y = new_mouse_y;

    // Zero mouse dx and dy unless the mouse is down
    if (!mouse_down) {
      mouse_dx = mouse_dy = 0;
    }

    // Update camera orientation
    rx += mouse_dy;
    ry += mouse_dx;

    // Render
    mat4f projection = mfrustum(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
    mat4f view = mid();
    view = mtranslate(&view, -x, y, -z);
    view = mrotatex(&view, rx / 1000.f);
    view = mrotatey(&view, ry / 1000.f);
    mat4f vp = mmulm(&view, &projection);

    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
    //GLfloat lightpos[] = {0.0f, 0.0f, -1.0f, 0.0f};
    //glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glLoadMatrixf(projection.m);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLoadMatrixf(view.m);

    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

    //gpu_mesh_draw(mesh);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
       0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
       3,                  // size
       GL_FLOAT,           // type
       GL_FALSE,           // normalized?
       0,                  // stride
       (void*)0            // array buffer offset
    );
    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDisableVertexAttribArray(0);

    window_swap(window);
  }

  window_delete(window);

  return 0;
}

