#ifndef PLANET_WINDOW_H
#define PLANET_WINDOW_H

struct Window;
union SDL_Event;

/// Create a new window with the specified parameters
struct Window* window_new(const char* title, int w, int h);

/// Destroy a window
void window_delete(struct Window* window);

/// Swap buffers
void window_swap(struct Window* window);

/// Peek next event (null if there are none left)
union SDL_Event* window_peek();


#endif

