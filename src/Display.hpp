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

	void fill(uint8_t* displayBuffer);
};
