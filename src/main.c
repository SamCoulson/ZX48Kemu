#include <stdio.h>
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "../include/raygui.h"
#include "../include/romloader.h"
#include "../include/screen.h"
#include "../include/cpu.h"
#include "../include/memory.h"
#include "../include/16-bit_load_group.h"

bool running = true;
bool paused = false;

int main(){

	// Re-direct output to console as SDL will disable it *Windows only*
	//freopen( "CON" ,"w", stdout);

	printf( "Output to console enabled\n");
 	
	initWindow();

	// Load the spectrum 48K ROM from file
	loadROMFile( "48.rom" );

	// Copy loaded system ROM data into system memory
	loadROM( romMemPtr, romFileSize );

	// Start at address 0x00
	//initCPU();
	initULA();

	// NOTE: no point in outputting every instruction that has been executed and will be executed as you can't read them
	// what I need is to be able to pause at will or on a certain address or condition
	// so why not have say -10 and +10 instructions from the current instuction being executed shown in a list
	// all I would need do is decode or translate 20 instuctions, I could then step the instructions and havbe then decoded as 
	// they flow into view.

	// Begin executing code from 0x00
	run( 0x0000 );

	CloseWindow();

	return 0;
}

//  Set the PC to point to a location in memory
void run( uint16_t addrs ){

	// Begin executing from 0x0000
	*reg->pc = addrs;

	// DEBUG
	*reg->pc = 0x0000;

	uint8_t timeToInterrupt = 200;


	//While less than 16k rom
	while( running ){

		if(WindowShouldClose()){
			exit(1);
		}

		updateScreen();

		if(paused)
		{
			continue;
		}
		
		// Execute the instruction pointed to by pc
		execute( &totalMem[*reg->pc]  );

		// Progess the PC by 1
		getNextByte();

		// Do interrupts ( Peripheral service routines )
		if( timeToInterrupt == 0x00 ){

			// Automatically save the current PC location on stack
			// Need to accomodate autoincrement of PC on return
			--*reg->pc;
			PUSH( getWordAt( reg->sp ), reg->sp, reg->pc );

			// INT - Software maskable interrupt
			// Responds depening on CPU Mode. ( See IM 0,1,2 in GPA )
			// 0 - As normal can execute instructions
			// 1 - Resets to 0038h instead of 0066h
			// 2 - Rediection can be made to anwhere in memory
			// When the IFF is reset the CPU cannot accpet the interrupt.
			// IFF1 actually hold the state and IFF2 is used as a temporary storage space
			// for IFF1 state.
			// EI enables ( Sets IFF1 to enabled )
			// DI disables ( Sets IFF2 to disabled )

			// Emulate ULA interrupting and reading video memory only if INT enabled
			if( *reg->iff1 != 0 ){
				readKeys();

			} // Emulate keyboard interrupt (Maskable interupt RST38)
			*reg->pc = 0x0038;


			// Emulate an NMI( Non-maskable interrupt ), is always honered
			// independent of IFF (interrupt enable flip-flop) status.  Always resets to 0066h
			// After an NMI on IFF1 is reset, IFF2 preserves the state as part of being able to restore
			// the CPU state before execution of NMI routine.
			// when LD A,I or LD A,R or RETN occurs the state of IFF2 is copied to
			// parut flag and can then be tested

			// Read video RAM
			readVideoRAM( totalMem );

			// Reset for next interrupt period
			timeToInterrupt = 200;

			// After accepting a maskable interrupt both IFFs are reset
			*reg->iff1 = 0;
			*reg->iff2 = 0;

		}else{
			//printf( "\nTime to interrupt = %d\n", timeToInterrupt );
			timeToInterrupt--;
		}

		// RESET - Set IFF1 and IIF2 to 0.

		// HALT - May also need to be implemented as it can be software generated, it issues some NOP's
		// while waiting for a NMI or interrupt request ( only is IIF is enabled )
		// R Register should be implemented, 7-bits are incremented after each instruction fetch
		// Computer memory becasue the capacitors holding 1/0 state loose charge and so need to be refreshed

		// Skip RAM-TEST - make PC skip to avoid the unnecassry check
		if( *reg->pc == 0x11DC ){
			*reg->pc = 0x11EF;
			// Set HL to +FFFF fro RAM-TOP
			*reg->hl = 0xFFFF;
		}
		
	}
}
