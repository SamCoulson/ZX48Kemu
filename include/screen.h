#ifndef INC_SCREEN_H
#define INC_SCREEN_H

#include "SDL/SDL.h"

int initSDL();

void updateScreen();

void putColor(SDL_Surface *surface, int x, int y, Uint32 color);

#endif // INC_SCREEN_H
