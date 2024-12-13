#include "raylib.h"
#include "../include/romloader.h"
#include "../include/screen.h"
#include "../include/cpu.h"
#include "../include/memory.h"
#include "../include/ULA.h"
#include <stdio.h>

int main(){

	// Re-direct output to console as SDL will disable it *Windows only*
	//freopen( "CON" ,"w", stdout);

	printf( "Output to console enabled\n");
 	
	initWindow();

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

	CloseWindow();

	return 0;
}
