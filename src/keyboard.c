#include "SDL/SDL.h"
#include "../include/keyboard.h"

extern SDL_Event event;

// For now just returns single chars back to ULA

// `Reads in keys that are pressed and returns a pointer to a map representing the keyboard
uint8_t keys[256];

uint8_t readKeyboard( uint8_t row ){

	uint8_t key = 0;

	if( event.type == SDL_KEYDOWN  ){
		
		switch( row ){
			case 1:{
				switch( event.key.keysym.sym ){
					case SDLK_z:
						key = 0x41; 	
						break;
					case SDLK_x:
						key = 0x41; 	
						break;
					case SDLK_c:
						key = 0x41; 	
						break;
					case SDLK_v:
						key = 0x41; 	
						break;
					default:
						break;
				}
			}
		}
		
	}else{
		key = 0x41;  // Should be 0x1F	
	}

	return key;

}
