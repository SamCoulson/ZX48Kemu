#include "../include/main.h"
#include <stdint.h>
#include <stdio.h>
#define RAYGUI_IMPLEMENTATION
#include "../include/raygui.h"
#include "../include/debugger.h"

int main()
{

	printf( "Output to console enabled\n");
 	
	initWindow();

	// Load the spectrum 48K ROM from file
	loadROMFile( "48.rom" );

	// Copy loaded system ROM data into system memory
	loadROM( romMemPtr, romFileSize );

	initDisassInstructionsBuffer();
	// Start at address 0x00
	//initCPU();
	initULA();

	// NOTE: no point in outputting every instruction that has been executed and will be executed as you can't read them
	// what I need is to be able to pause at will or on a certain address or condition
	// so why not have say -10 and +10 instructions from the current instuction being executed shown in a list
	// all I would need do is decode or translate 20 instuctions, I could then step the instructions and havbe then decoded as 
	// they flow into view.
	printf("about to run\n");	

	run( 0x0000 );

	CloseWindow();

	return 0;
}

//  Set the PC to point to a location in memory
void run( uint16_t addrs )
{
	printf("Entered run\n");

	// Begin executing from 0x0000
	*z80->pc = addrs;
	printf("address = %04X, byte at %02X\n", *z80->pc, totalMem[*z80->pc]);
	// DEBUG
	*z80->pc = 0x0000;

	struct timespec start, end;

	int screenUpdateCount = 0;

	clock_gettime(CLOCK_MONOTONIC, &start);

	paused = true;

	while( !WindowShouldClose() )
	{	
/*printf("inside while");*/
		checkBreakPointHit(*z80->pc);

		if(paused)
		{
			clock_gettime(CLOCK_MONOTONIC, &start);
			updateScreen();

			if(!stepping)
			{
				continue;
			}
		}

		if( t_counter > 70000)
		{
		        readVideoRAM( totalMem ); 
			updateScreen();
			screenUpdateCount++;
		}

		printf("just about to execute\n");

		// Execute the instruction pointed to by pc
		uint8_t* val = &totalMem[*z80->pc];
		printf("mem val %0x\n", *val);
		/*execute( &totalMem[*z80->pc]  );*/
		execute( val );

		// Progess the PC by 1
		getNextByte();

		clock_gettime(CLOCK_MONOTONIC, &end);

		uint64_t elapsed_ns = (uint64_t)(end.tv_sec - start.tv_sec) * 1000000000ULL + (end.tv_nsec - start.tv_nsec);

		double elapsed_s = elapsed_ns / 1e9; // Convert to seconds
		
		if(elapsed_s > 1.0)
		{
			printf("Elapsed time: %ld4ns (%f seconds)\n", elapsed_ns, elapsed_s);
			printf("********************t_counter = %d\n", t_counter);
			printf("screen udpated : %d\n", screenUpdateCount );
			clock_gettime(CLOCK_MONOTONIC, &start);
			t_counter = 0;
			screenUpdateCount = 0;
		}

		stepping = false;
	}
}

// Do interrupts ( Peripheral service routines )
void doInterrupt()
{
//		if( timeToInterrupt == 0x00 ){
//			// Automatically save the current PC location on stack
//			// Need to accomodate autoincrement of PC on return
//			--*reg->pc;
//			PUSH( getWordAt( reg->sp ), reg->sp, reg->pc );
//
//			// INT - Software maskable interrupt
//			// Responds depening on CPU Mode. ( See IM 0,1,2 in GPA )
//			// 0 - As normal can execute instructions
//			// 1 - Resets to 0038h instead of 0066h
//			// 2 - Rediection can be made to anwhere in memory
//			// When the IFF is reset the CPU cannot accpet the interrupt.
//			// IFF1 actually hold the state and IFF2 is used as a temporary storage space
//			// for IFF1 state.
//			// EI enables ( Sets IFF1 to enabled )
//			// DI disables ( Sets IFF2 to disabled )
//
//			// Emulate ULA interrupting and reading video memory only if INT enabled
//			if( *reg->iff1 != 0 ){
//				readKeys();
//
//			} 
//
//			// Emulate keyboard interrupt (Maskable interupt RST38)
//			// setting pc to 0x0038 simulates a keyboard event
//			//*reg->pc = 0x0038;
//
//
//			// Emulate an NMI( Non-maskable interrupt ), is always honered
//			// independent of IFF (interrupt enable flip-flop) status.  Always resets to 0066h
//			// After an NMI on IFF1 is reset, IFF2 preserves the state as part of being able to restore
//			// the CPU state before execution of NMI routine.
//			// when LD A,I or LD A,R or RETN occurs the state of IFF2 is copied to
//			// parut flag and can then be tested
//
//			// Read video RAM
//			readVideoRAM( totalMem );
//
//			// Reset for next interrupt period
//			timeToInterrupt = 10000;
//
//			// After accepting a maskable interrupt both IFFs are reset
//			*reg->iff1 = 0;
//			*reg->iff2 = 0;
//		}
//		else
//		{
//			timeToInterrupt--;
//		}
//
		// RESET - Set IFF1 and IIF2 to 0.

		// HALT - May also need to be implemented as it can be software generated, it issues some NOP's
		// while waiting for a NMI or interrupt request ( only is IIF is enabled )
		// R Register should be implemented, 7-bits are incremented after each instruction fetch
		// Computer memory becasue the capacitors holding 1/0 state loose charge and so need to be refreshed
}
