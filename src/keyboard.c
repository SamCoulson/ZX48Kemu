#include "SDL/SDL.h"
#include "../include/keyboard.h"

extern SDL_Event event;

void readKeyboard(){
	if( event.type == SDL_KEYDOWN  ){
		switch( event.key.keysym.sym ){
			case SDLK_a:
				break;
			default:
				break;
		}
	}

}
