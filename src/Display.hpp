#pragma once
#include "config.h"
#include <_types/_uint8_t.h>
#include <SDL2/SDL.h>

class Display
{
public:
	SDL_Surface *windowSurface;
	SDL_Window *window;
    Display();
    ~Display();
	void drawRectangle(int sx, int sy, int w, int h);
	void draw7Segments(int x, int y, int w, int h, bool lines[7]);
	void number(int x, int y, int w, int h, int n);
	void dfloat(int x, int y, int w, int h, float n);

	void fill(uint8_t* displayBuffer);
};
