#include "../include/ULA.h"
#include "../include/memory.h"
#include "../include/screen.h"
#include "../include/keyboard.h"
#include <stdio.h>

extern uint8_t ports[256];

void readKeys(){
	// Copy any pressed key values to input port
	ports[0xFE] = readKeyboard();	
}

void readVideoRAM( unsigned char* mem ){

	updateScreen();	

}
