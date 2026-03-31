#ifndef INC_MEMORY_H
#define INC_MEMORY_H

#include <stdint.h>

// Loads the system ROM data into memory
extern int loadROMintoMemory(uint8_t *src, unsigned int noBytes);

// Output the ROM file contents to a text file
void memDump();

// Returns a pointer to the requested 16-bit address in memory
uint8_t *getByte(uint16_t addrs);

uint16_t read_word(uint16_t addrs);
uint16_t *getWord(uint16_t addrs);

// Writes an 8-bit value to the 16-bit address location in memory
void write_byte(uint16_t loc, uint8_t val);

void write_word(uint16_t addr, uint16_t value);

// Total 64K Spectrum memory
extern unsigned char memory[65536];

#endif // INC_MEMORY_H
