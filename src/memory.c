#include "../include/memory.h"

// Copy the ROM file data into sysyem memory
int loadROM( unsigned char* src, unsigned int noBytes){

	if( src == NULL ){
		printf( "Unable to load ROM data into memory\n" );
		return 1;
	}
	
	for(int i = 0; i < noBytes; i++){
		totalMem[i] = src[i];
	}
	printf("ROM file loaded into memory\n");
	return 0;
}

void memDump(){

	// pointer to start of ROM memory
	unsigned char* ROM;

	// Point at beginning of memory
	ROM = totalMem;	

	printf( "Size of memory is %d KBytes\n", sizeof(totalMem)/1024);

	for(int i = 0; i < 5/*sizeof(totalMem)*/; i++ ){
		if(ROM[i] == 0x0d)
			ROM[i] = 'D';
		if(ROM[i] == 0x09)
			ROM[i] = '9';
		if(ROM[i] == 0x10)
			ROM[i] = '1';
		if(ROM[i] == 0x0a)
			ROM[i] = 'a';

		//std::bitset<8> b1(ch);
		printf( "Byte %d\t%04X\t\n",i ,totalMem[i] );
	}
	
}
