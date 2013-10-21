#ifndef INC_MEMORY_H
#define INC_MEMORY_H

#include <iostream>
#include <stdint.h>

class Memory{
	public:
		Memory();
		~Memory();

	// Returns the start address of the memory
	unsigned char* getMem(){return totalMem;}

	// Loads the system ROM data into memory
	bool loadROM(uint8_t startAddrs, uint8_t* src, unsigned int noBytes);

	// Output the ROM file contents to a text file
	void dumpMemToFile(const std::string filename);

	// Returns a pointer to the requested 16-bit address in memory
	uint8_t* getByte(uint16_t addrs){return &totalMem[addrs];}

	// Writes an 8-bit value to the 16-bit location in memory
	void setByte(uint16_t loc, uint8_t val){totalMem[loc] = val;}

	private:

	// Total 64K Spectrum memory
	uint8_t totalMem[65536];

};


#endif // INC_MEMORY_H
