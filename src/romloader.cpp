#include <fstream>
#include <bitset>
#include "../include/romloader.h"

ROMLoader::~ROMLoader(){
	delete[] romMemPtr;
}

bool ROMLoader::loadROMFile(const std::string filename){

	// Open file in binary mode i.e. non-formatted,to read in data, place the stream pointer at the end of the file  
	std::ifstream romFile(filename.c_str(), std::ios::in | std::ios::binary | std::ios::ate);

	// Hold the size of the file
	std::streamoff size;

	// Test for succesful file access
	if(romFile.is_open()){
	
		// Get the streampointer position (should reveal how long the file is in bytes)
		size = romFile.tellg();

		// Allocate memory for charcters read in
		romMemPtr = new uint8_t[(int)size+1];

		// Place the stream pointer to beginning of file read to begin reading in the data
		romFile.seekg(0, std::ios::beg);

		// Read in all the data into the memory buffer
		romFile.read((char*)romMemPtr, size);

		// Add termination character
		romMemPtr[size+1] = '\0';

		// Close the file
		romFile.close();
	}else{

		// Could not open file return failed
		return false;
	}

	ROMFileSize = size;

	return true;
	
}

void ROMLoader::dumpROMData(){
	std::cout << "Size of ROM file is " << getSize()/1024 << " Kbytes" << std::endl;

	uint8_t* ROM = getROMData();

	for(int i = 0; i < 200; i++ ){
		int ch = (uint8_t)ROM[i]; 
		std::bitset<8> b1(ch);
		std::cout << std::dec << "Byte " << "\t" << i << "\t" <<
			(int)ROM[i] << "\t" << 
			ROM[i] << "\t" << 
			std::hex << std::showbase << "\t" <<
			(unsigned int)ROM[i] << "\t" <<
			b1 << "\t" <<
			(unsigned int)&ROM[i] << std::endl;
	}

}
