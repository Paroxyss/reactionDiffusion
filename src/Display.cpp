#include "Display.hpp"
#include "config.h"
#include <iostream>

Display::Display() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Failed to initialize the SDL2 library\n";
        return;
    }

    window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    if (!window) {
        std::cout << "Failed to create window\n";
        return;
    }

    windowSurface = SDL_GetWindowSurface(window);

    if (!windowSurface) {
        std::cout << "Failed to get the surface from the window\n";
        return;
    }

    SDL_UpdateWindowSurface(window);
}
Display::~Display() {}

void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel) {
    Uint32 *const target_pixel =
        (Uint32 *)((Uint8 *)surface->pixels + y * surface->pitch +
                   x * surface->format->BytesPerPixel);
    *target_pixel = pixel;
}

void Display::fill(uint8_t *displayBuffer) {
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
			Uint32 pixel = displayBuffer[3*(x+WIDTH*y)] << 16;
			pixel += displayBuffer[3*(x+WIDTH*y) + 1] << 8;
			pixel += displayBuffer[3*(x+WIDTH*y) + 2];
			set_pixel(windowSurface, x, y, pixel);
        }
    }
    SDL_UpdateWindowSurface(window);
}
