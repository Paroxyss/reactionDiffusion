#include "Display.hpp"
#include "Simulation.hpp"
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
            Uint32 pixel = displayBuffer[3 * (x + WIDTH * y)] << 16;
            pixel += displayBuffer[3 * (x + WIDTH * y) + 1] << 8;
            pixel += displayBuffer[3 * (x + WIDTH * y) + 2];
            set_pixel(windowSurface, x, y, pixel);
        }
    }
	reactionParameters* params = (reactionParameters*)parameters->contents();
	const int vEcart = 50;
	const int fontSize = 20;
	dfloat(10, 10, fontSize, fontSize, params->diffA);
	dfloat(10, 10 + vEcart, fontSize, fontSize, params->diffB);
	dfloat(10, 10 + 2*vEcart, fontSize, fontSize, params->feedA);
	dfloat(10, 10 + 3*vEcart, fontSize, fontSize, params->killB);
    SDL_UpdateWindowSurface(window);
    /*SDL_Surface *imageSurface =
        SDL_CreateRGBSurfaceFrom(displayBuffer, WIDTH, HEIGHT,
                                 sizeof(unsigned char) * 3, // depth
                                 WIDTH * 3,  // pitch (row length * BPP)
                                 0x000000ff, // red mask
                                 0x0000ff00, // green mask
                                 0x00ff0000, // blue mask
                                 0);

    SDL_Rect srcClip;
    srcClip.x = 0;
    srcClip.y = 0;
    srcClip.w = WIDTH;
    srcClip.h = HEIGHT;

    SDL_Rect dstClip;
    dstClip.x = 0;
    dstClip.y = 0;
    dstClip.w = WIDTH;
    dstClip.h = HEIGHT;

    SDL_BlitSurface(imageSurface, &srcClip, windowSurface, &dstClip);*/
}

void Display::drawRectangle(int sx, int sy, int w, int h) {
    for (int x = sx; x < sx + w; x++) {
        for (int y = sy; y < sy + h; y++) {
            set_pixel(windowSurface, x, y, 255<<8);
        }
    }
}
void Display::draw7Segments(int x, int y, int w, int h, bool lines[7]) {
    for (int i = 0; i < 3; i++) {
        if (lines[i]) {
            drawRectangle(x, y + i * (h / 2), w, h / 8);
        }
    }
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            if (lines[3 + i + 2 * j]) {
                drawRectangle(x + j * w, y + i * (h / 2), w / 8, h / 2);
            }
        }
    }
}

bool sevenSegments[7 * 10] = {
    1, 0, 1, 1, 1, 1, 1, // 0
    0, 0, 0, 1, 1, 0, 0, // 1
    1, 1, 1, 0, 1, 1, 0, // 2
    1, 1, 1, 0, 0, 1, 1, // 3
    0, 1, 0, 1, 0, 1, 1, // 4
    1, 1, 1, 1, 0, 0, 1, // 5
    1, 1, 1, 1, 1, 0, 1, // 6
    1, 0, 0, 0, 0, 1, 1, // 7
    1, 1, 1, 1, 1, 1, 1, // 8
    1, 1, 1, 1, 0, 1, 1, // 9
};

void Display::number(int x, int y, int w, int h, int n) {
    if (!(0 <= n && n < 10))
        return;
    draw7Segments(x, y, w, h, &sevenSegments[7 * n]);
}
void Display::dfloat(int x, int y, int w, int h, float n) {
	for(int i = 0; i < 5; i++){
		number(x + (h*1.2) * i, y, w, h, ((int)n)%10);
		n*=10;
	}
}
