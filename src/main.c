#include "../include/CPU.h"
#include "../include/Memory.h"
#include "../include/romloader.h"
#include <stdio.h>


int main(int argc, char *argv[] ){	

	// Load the spectrum 48K ROM from file
	loadROMFile( "48.rom" );

	// Copy loaded system ROM data into system memory 
	loadROM( romMemPtr, romFileSize );	

	// Start at address 0x00
	initCPU();

	// Begin executing code from 0x00
	run( 0x11E2 );

	getchar();

	return 0;
}
