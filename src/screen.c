#include "../include/screen.h"

SDL_Surface* screen = NULL;

int initSDL(){

	//Start SDL
	int result = SDL_Init( SDL_INIT_EVERYTHING );

	if(result != 0)
	{
		printf("Could not initialise SDL\n");
		return 1;
	}

	 //Set up screen
	screen = SDL_SetVideoMode( 256, 192, 8, SDL_HWSURFACE );

	//Apply image to screen
	//SDL_BlitSurface( hello, NULL, screen, NULL );

	//Update Screen
	SDL_Flip( screen );

	return 0;
}

void updateScreen(){


}


