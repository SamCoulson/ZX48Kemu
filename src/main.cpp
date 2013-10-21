#include <iostream>
#include "../include/CPU.h"
#include "../include/Memory.h"
#include "../include/romloader.h"
#include "windows.h"
#include "stdio.h"

int main(int argc, char *argv[] ){

	
	Memory *specMem = new Memory();
	ROMLoader *specROM = new ROMLoader();
	CPU *specCPU = new CPU(specMem);

	// Load the spectrum 48K ROM from file
	if(!specROM->loadROMFile("48.rom")){
		std::cout << "Failed to open ROM file!" << std::endl;
	}

	// Copy loaded system ROM data into system memory start at address 0x00
	if(!specMem->loadROM( 0, specROM->getROMData(), specROM->getSize() )){
		std::cout << "Failed to load ROM file data in to system memory!" << std::endl;
	}

	// specMem->dumpMemToFile("memDump.txt");

	// Begin executing code from 0x00
	specCPU->start();

	getchar();

	return 0;
}
