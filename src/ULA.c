#include "../include/ULA.h"
#include "../include/memory.h"
#include <stdio.h>

void read( uint8_t* port ){
	printf("Read %X at port\n", *port);	
}

void readVideoRAM( unsigned char* mem ){
	// Read the video memory
	for( int i = 0x4000; i < 0x5AFF; i++ ){
		printf( "%X\n", i );
	}

}
