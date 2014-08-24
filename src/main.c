#include "SDL/SDL.h"
#include "../include/CPU.h"
#include "../include/ULA.h"
#include "../include/Memory.h"
#include "../include/romloader.h"
#include "../include/screen.h"
#include <stdio.h>

int main(int argc, char *argv[] ){	
	
	// Re-direct output to console as SDL will disable it	
	freopen( "CON" ,"w", stdout);

	printf( "Output to console enabled\n");

	initSDL();

	// Load the spectrum 48K ROM from file
	loadROMFile( "48.rom" );

	// Copy loaded system ROM data into system memory 
	loadROM( romMemPtr, romFileSize );	

	// Start at address 0x00
	//initCPU();
	initULA();
	// Begin executing code from 0x00
	run( 0x0000 );

	getchar();

	SDL_Quit();
	
	return 0;
}
