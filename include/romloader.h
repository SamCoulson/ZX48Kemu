#ifndef INC_ROMLOADER_H
#define INC_ROMLOADER_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Size of ROM file
extern int romFileSize; 

// Loads the ROM image into the memory
extern int loadROMFile( const char* filename );

// Returns a pointer to the beginning of the loaded data
uint8_t* getROMData();

// Return the size of the ROM file in bytes
unsigned int getSize();

// Dumps the content of the ROM data to the console window (NOTE: Make it output to a text file)
void dumpROMData();

// Pointer to memory holding ROM data
// ROM memory pointer
extern unsigned char *romMemPtr; 

#endif // INC_ROMLOADER_H
