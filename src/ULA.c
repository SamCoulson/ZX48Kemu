#include "../include/cpu.h"
#include "../include/ULA.h"
#include "../include/memory.h"
#include "../include/screen.h"
#include "../include/keyboard.h"
#include <stdio.h>

extern uint8_t ports[256];

extern uint8_t halfRows[8];

uint8_t ulaPort( int ioStatus, uint8_t addrs ){
	if( ioStatus == 0 ){
		//printf("Running ULA instruction as READ\n");
		// Return the specified row
		switch( addrs ){
			case 0xFE:
				return halfRows[0];
				break;
			case 0xFD:
				return halfRows[1];
				break;
			case 0xFB:
				return halfRows[2];
				break;
			case 0xF7:
				return halfRows[3];
				break;
			case 0xEF:
				return halfRows[4];
				break;
			case 0xDF:
				return halfRows[5];
				break;
			case 0xBF:
				return halfRows[6];
				break;
			case 0x7F:
				return halfRows[7];
				break;
			default:
				break;
		}
	}else{
		printf("Running ULA instruction as WRITE\n");
	}
	return 0xBF;
}

void initULA(){
	mapPort( 254, &ulaPort );
}


// readKeys() should be called outside of the CPU
void readKeys(){
	// Read the keys from the keyboard and set the appropriate bits in each of the half row bytes.
	readKeyboard();
}

// Potentially have this as a way to emulate the ULA sending a clock tick to the CPU
// however its not a clock tick in the sense of an operation per tick but instead an instruction per tick
// maybe able to ties this to a timer somehow.
// void execute( opCode ){
//
//
// }

void readVideoRAM( unsigned char* mem ){

	updateScreen();

}
