#ifndef INC_ROMLOADER_H
#define INC_ROMLOADER_H

#include <iostream>
#include <stdint.h>

class ROMLoader{

	public:
		ROMLoader(){};
		~ROMLoader();

		// Loads the ROM image into the memory
		bool loadROMFile( const std::string filename );

		// Returns a pointer to the beginning of the loaded data
		uint8_t* getROMData(){return romMemPtr;}

		// Return the size of the ROM file in bytes
		unsigned int getSize(){return ROMFileSize;}

		// Dumps the content of the ROM data to the console window (NOTE: Make it output to a text file)
		void dumpROMData();

	private:

		// Pointer to memory holding ROM data
		uint8_t* romMemPtr;

		// Size of ROM file in bytes
		unsigned int ROMFileSize;
};

#endif // INC_ROMLOADER_H
