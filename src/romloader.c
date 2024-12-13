#include "../include/romloader.h"

unsigned char *romMemPtr; 
int romFileSize; 

int loadROMFile(const char* filename){

	// ROMfile handle
	FILE *romFile;

	// Open file in binary mode
	romFile = fopen( filename, "r" );

	// Hold the size of the file
	size_t size;

	// Test for succesful file access
	if(romFile != 0){
	
		// Get the streampointer position (should reveal how long the file is in bytes)
		fseek( romFile, 0, SEEK_END );

		// Determine file length
		size = ftell( romFile );

		// Place the file postion pointer to beginning of file read to begin reading in the data
		fseek( romFile, 0, SEEK_SET );

		// Allocate memory
		romMemPtr = (unsigned char *)malloc( size+1 );
		if( !romMemPtr ){
			printf("Unable to allocate memory\n");
			fclose( romFile );
			return 1;	
		}

		// Read in all the data into the memory buffer
		const size_t ret_code = fread( romMemPtr, size, 1, romFile );

		if (ret_code == size)
		{
			printf("Successfully read rom in to memory\n");
		}	

		// Add termination character
		romMemPtr[size+1] = '\0';

		// Store the size of the file
		romFileSize = size+1;

		// Close the file
		fclose( romFile );
	}else{
		// Could not open file return failed
		printf("Failed to open file\n");
		return 1;
	}

	return 0;
}

unsigned int ROMFileSize(){
	return romFileSize; 
}

// Is supposed to dump the contents of ROm data showing binra, char and address info

void dumpROMData(){
	printf( "Size of ROM file is" ); // getSize()/1024 Kbytes"\n);

	int i = 0;
	for(i = 0; i < 200; i++ ){
		
		printf("%02X\n", romMemPtr[i]);
	}

}

