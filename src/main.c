#include "SDL/SDL.h"
#include "../include/CPU.h"
#include "../include/Memory.h"
#include "../include/romloader.h"
#include <stdio.h>

int main(int argc, char *argv[] ){	
	
	// Re-direct output to console as SDL will disable it	
	freopen( "CON" ,"w", stdout);

	printf( "Output to console enabled\n" );
	
	/*
	//Apply image to screen
	SDL_BlitSurface( hello, NULL, screen, NULL );

	//Update Screen
	SDL_Flip( screen );
	
	//Pause
	SDL_Delay( 5000 );
	*/

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
