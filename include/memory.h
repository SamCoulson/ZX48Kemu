#ifndef INC_MEMORY_H
#define INC_MEMORY_H

#include <stdio.h>
#include <stdint.h>

// Loads the system ROM data into memory
int loadROM( uint8_t* src, unsigned int noBytes);

// Output the ROM file contents to a text file
void memDump();

// Returns a pointer to the requested 16-bit address in memory
uint8_t* getByte( uint16_t addrs );

uint16_t* getWord( uint16_t addrs );

// Writes an 8-bit value to the 16-bit address location in memory
void setByte(uint16_t loc, uint8_t val);

// Total 64K Spectrum memory
extern unsigned char totalMem[65536];

#endif // INC_MEMORY_H
