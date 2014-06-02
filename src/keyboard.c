#include "SDL/SDL.h"
#include "../include/keyboard.h"

extern SDL_Event event;

// For now just returns single chars back to ULA

uint8_t readKeyboard(){

	uint8_t key = 0;

	if( event.type == SDL_KEYDOWN  ){
		switch( event.key.keysym.sym ){
			case SDLK_a:
				key = 0x41; 	
				break;
			default:
				break;
		}
	}else{
		key = 0x45;	
	}

	return key;

}
