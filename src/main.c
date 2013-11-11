#include "SDL/SDL.h"
#include "../include/CPU.h"
#include "../include/Memory.h"
#include "../include/romloader.h"
#include <stdio.h>

int main(int argc, char *argv[] ){	
	
	SDL_Surface* screen = NULL;
	SDL_Surface* hello = NULL;
		
	//Start SDL
	int result = SDL_Init( SDL_INIT_EVERYTHING );
    
	if(result != 0)
	{
		printf("Could not initialise SDL\n");
	}
	
	 //Set up screen
	screen = SDL_SetVideoMode( 256, 192, 8, SDL_HWSURFACE );
	
	freopen( "CON" ,"w", stdout);

	printf( "out to console\n" );

	//Apply image to screen
	SDL_BlitSurface( hello, NULL, screen, NULL );

	//Update Screen
	SDL_Flip( screen );

	//Pause
	//SDL_Delay( 5000 );

	// Load the spectrum 48K ROM from file
	loadROMFile( "48.rom" );

	// Copy loaded system ROM data into system memory 
	loadROM( romMemPtr, romFileSize );	

	// Start at address 0x00
	initCPU();

	// Begin executing code from 0x00
	run( 0x0000 );

	getchar();

	SDL_Quit();
	
	return 0;
}
