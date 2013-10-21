#include <bitset>
#include <fstream>
#include "../include/memory.h"

Memory::Memory(){
	
}

Memory::~Memory(){
	delete [] totalMem;
}

// Copy the ROM file data into sysyem memory
bool Memory::loadROM(uint8_t startAddrs, uint8_t* src, unsigned int noBytes){

	if( (startAddrs > 0 ) || (src == NULL)){
		return false;
	}

	for(unsigned int i = 0; i < noBytes; i++){
		totalMem[startAddrs+i] = src[i];
	}

	return true;
}

void Memory::dumpMemToFile(const std::string filename){

	std::ofstream memDumpFile; 

	memDumpFile.open(filename.c_str(), std::ios::out );

	unsigned char* ROM = getMem();

	//memDumpFile << "Size of ROM file is " << sizeof(totalMem)/1024 << " Kbytes" << std::endl;

	if(memDumpFile.is_open()){
		for(unsigned int i = 0; i < sizeof(totalMem); i++ ){
			if(ROM[i] == 0x0d)
				ROM[i] = 'D';
			if(ROM[i] == 0x09)
				ROM[i] = '9';
			if(ROM[i] == 0x10)
				ROM[i] = '1';
			if(ROM[i] == 0x0a)
				ROM[i] = 'a';
			char ch = ROM[i]; 
			std::bitset<8> b1(ch);
			memDumpFile << std::dec << "Byte " << "\t" << i << "\t" <<
				(int)ROM[i] << "\t" << 
				ROM[i] << "\t" << 
				std::hex << std::showbase << "\t" <<
				(unsigned int)ROM[i] << "\t" <<
				b1 << "\t" <<
				(unsigned int)&ROM[i] << std::endl;
		}
	}
	
}
