#include "../include/ULA.h"
#include "../include/memory.h"
#include <stdio.h>

void read( uint8_t* port ){
	printf("Read %X at port\n", *port);	
}

void readVideoRAM( unsigned char* mem ){

	// Row counter
	uint8_t column = 0;

	// Read the video memory
	for( int i = 0x4000; i < 0x57FF; i+=8 ){
		printf( "%X", mem[i] );
		column++;
		if( column == 32 ){
			printf("\n");
			column = 0;
		}
	}

}
