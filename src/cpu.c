#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "../include/cpu.h"
#include "../include/8-bit_load_group.h"
#include "../include/16-bit_load_group.h"
#include "../include/GPA_and_CPU_control_group.h"
#include "../include/16-bit_arithmetic_group.h"
#include "../include/rotate_and_shift_group.h"
#include "../include/8-bit_arithmetic_group.h"
#include "../include/BSR_and_test_group.h"
#include "../include/jump_group.h"
#include "../include/call_and_return_group.h"
#include "../include/IO_group.h"
#include "../include/EBTS_group.h"
#include "../include/util_bit_operations.h"
#include "../include/ULA.h"
#include "../include/console.h"

#define BUF_SIZE 5

// Declare and initialise structure of pointers to register unions, this must be here not in the .h file
static Registers registers = {
        // Assign const pointers to main registers	
	.a = &af._af[1], .f = &af._af[0], .af = &af.af,
	.b = &bc._bc[1], .c = &bc._bc[0], .bc = &bc.bc,
	.d = &de._de[1], .e = &de._de[0], .de = &de.de,
	.h = &hl._hl[1], .l = &hl._hl[0], .hl = &hl.hl,
	// Assign const pointers to alternative registers 
	.alta = &altaf._af[1], .altf = &altaf._af[0], .altaf = &altaf.af,
	.altb = &altbc._bc[1], .altc = &altbc._bc[0], .altbc = &altbc.bc,
	.altd = &altde._de[1], .alte = &altde._de[0], .altde = &altde.de,
	.alth = &althl._hl[1], .altl = &althl._hl[0], .althl = &althl.hl,
	// 8-bit Special registers - interrupt vector and refresh register
	.i = &ir._ir[1], .r = &ir._ir[0], .ir = &ir.ir,
	// 16-bit index registers
	.ix = &ix, .iy = &iy,
      	// 16-bit stack pointer and program pointer
	.sp = &sp, .pc = &pc,
	// IFF (Interrupt enabled flip-flop)
	.iff1 = &iff1, .iff2 = &iff2,
	// CPU Mode
	.mode = &mode	
};

// Pointer to the register structure
Registers* reg = &registers;

// IO ports *** The CPU has 256 addressable ports *** 
// This might be excessive for emualtion but all IO will park data at these location for read/write by external modules

uint8_t ports[256] = {0};
static uint8_t* IOport = ports; 

//  Set the PC to point to a location in memory
void run( uint16_t addrs ){
	
	// Begin executing from 0x0000
	*reg->pc = addrs;

	// DEBUG
	*reg->pc = 0x0000;
	
	uint8_t timeToInterrupt = 200;

	//While less than 16k rom
	while( *reg->pc < 0xFFFF ){			
		
		console();	

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
			//if( *reg->iff1 != 0 ){
				readKeys();
			//}
		
			// Emulate keyboard interrupt (Maskable interupt RST38)
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

		// R Register should be implemented, 7-bits are incremetend after each instrutrion fetch
	
		// computer memory becasue the capacitors holding 1/0 state loose charge and so need to be refreshed	

		// Skip RAM-TEST - make PC skip to avoid the unnecassry check
		if( *reg->pc == 0x11DC ){
			*reg->pc = 0x11EF;
			// Set HL to +FFFF fro RAM-TOP
			*reg->hl = 0xFFFF;	
		}
	}
}


void execute( uint8_t* opcode ){	

	uint8_t offset;	

	printf("0x%04X\t%02X\t", *reg->pc, totalMem[*reg->pc] );
	
	switch( *opcode ){
		case 0x00:
			printf( "NOP (Not implemented yet!");
			NOP();
			break;
		case 0x01:
			printf( "LD BC,+%X", readNextWord() );
			LD16( reg->bc, getNextWord() );
			break;
		case 0x02:
			printf( "LD(BC),A" );
			LD( getByteAt( *reg->bc ) ,reg->a );
			break;
		case 0x03:
			printf( "INC BC" );
			INC16( reg->bc );
			break;
		case 0x04:
			printf( "INC B" );
			INC( reg->b, reg->f );
			break;
		case 0x05:
			printf( "DEC B" );
			DEC( reg->b, reg->f );
			break;
		case 0x06:
			printf( "LD B,+%X", readNextByte() );
			LD( reg->b, getNextByte() );
			break;
		case 0x07:
			printf( "RLCA" );
			RLCA( reg->a, reg->f );
			break;
		case 0x08:
			printf( "EX AF,AF'" );
			EX( reg->af, reg->altaf );
			break;
		case 0x09:
			printf("ADD HL,BC");
			ADD16( reg->hl, reg->bc, reg->f );
			break;
		case 0x0A:
			printf( "LD A,(BC)" );
			LD( reg->a, getByteAt( *reg->bc ) );
			break;
		case 0x0B:
			printf( "DEC BC" );
			DEC16( reg->bc );
			break;
		case 0x0C:
			printf( "INC C" );
			INC( reg->c, reg->f );
			break;
		case 0x0D:
			printf( "DEC C" );
			DEC( reg->c, reg->f );
			break;
		case 0x0E:
			printf( "LD C,+%X", readNextByte() );
			LD( reg->c, getNextByte() );
			break;
		case 0x0F:
			printf( "RRCA" );
			RRCA( reg->a, reg->f );
			break;
		case 0x10:
			printf( "DJNZ %d", (int8_t)readNextByte() );
			DJNZ( getNextByte(), reg->b, reg->pc );					
			break;
		case 0x11:
			printf( "LD DE,+%X", readNextWord() );
			LD16( reg->de, getNextWord() );
			break;
		case 0x12:
			printf( "LD(DE),A");
			LD( getByteAt( *reg->de ) ,reg->a );
			break;
		case 0x13:
			printf( "INC DE" );
			INC16( reg->de );
			break;
		case 0x14:
			printf( "INC D" );
			INC( reg->d, reg->f );
			break;
		case 0x15:
			printf( "DEC D" );
			DEC( reg->d, reg->f );
			break;
		case 0x16:
			printf( "LD D,+%X", readNextByte() );
			LD( reg->d, getNextByte() );
			break;
		case 0x17:
			printf( "RLA" );
			RLA( reg->a, reg->f );
			break;
		case 0x18:
			printf( "JR %X", readNextByte() );
			JR( getNextByte(), reg->pc );
			break;
		case 0x19:
			printf("ADD HL,DE");
			ADD16( reg->hl, reg->de, reg->f );
			break;
		case 0x1A:
			printf( "LD A,(DE)" );
			LD( reg->a, getByteAt( *reg->de ) );
			break;
		case 0x1B:
			printf( "DEC DE" );
			DEC16( reg->de );
			break;
		case 0x1C:
			printf( "INC E" );
			INC( reg->e, reg->f );
			break;
		case 0x1D:
			printf( "DEC E" );
			DEC( reg->e, reg->f );
			break;
		case 0x1E:
			printf( "LD E,+%X", readNextByte() );
			LD( reg->e, getNextByte() );
			break;
		case 0x1F:
			printf( "RRA" );
			RRA( reg->a, reg->f );
			break;
		case 0x20:
			printf( "JR NZ,%d", (int8_t)readNextByte() );
			JRNZ( reg->pc, getNextByte(), reg->f );
			break;
		case 0x21:
			printf( "LD HL,+%X", readNextWord() );
			LD16( reg->hl, getNextWord() );
			break;
		case 0x22:
			printf( "LD (%X),HL", readNextWord() );
			LD16( getWordAt( getNextWord() ), reg->hl );							
			break;
		case 0x23:
			printf( "INC HL" );
			INC16( reg->hl );
			break;
		case 0x24:
			printf( "INC H" );
			INC( reg->h, reg->f );
			break;
		case 0x25:
			printf( "DEC H" );
			DEC( reg->h, reg->f );
			break;
		case 0x26:
			printf( "LD H,+%X", readNextByte() );
			LD( reg->h, getNextByte() );
			break;/*
		case 0x27:
			printf( "DAA" );
			break;*/
		case 0x28:
			printf( "JR Z,+%X", readNextByte() );
			JRZ( reg->pc, getNextByte(), reg->f );
			break;
		case 0x29:
			printf("ADD HL,HL");
			ADD16( reg->hl, reg->hl, reg->f );
			break;
		case 0x2A:
			printf( "LD HL,(%X)", readNextWord() ); 
			LD16( reg->hl, getWordAt( getNextWord() ) );
			break;
		case 0x2B:
			printf( "DEC HL");
			DEC16( reg->hl );
			break;
		case 0x2C:
			printf( "INC L" );
			INC( reg->l, reg->f );
			break;
		case 0x2D:
			printf( "DEC L" );
			DEC( reg->l, reg->f );
			break;
		case 0x2E:
			printf( "LD L,+%X", readNextByte() );
			LD( reg->l, getNextByte() );
			break;
		case 0x2F:
			printf( "CPL" );
			CPL( reg->a, reg->f );
			break;
		case 0x30:
			printf("JR NC,%d", readNextByte() );
			JRNC( reg->pc, getNextByte(), reg->f );
			break;
		case 0x31:
			printf( "LD SP,+%X", readNextWord() );
			LD16( reg->hl, getNextWord() );
			break;
		case 0x32:
			printf( "LD (%X),A", readNextWord() );
			LD( getByteAt( *( getNextWord() ) ) ,reg->a );
			break;
		case 0x33:
			printf( "INC SP" );
			INC16( reg->sp );
			break;
		case 0x34:
			printf( "INC (HL)" );
			INC( getByteAt( *reg->hl ), reg->f );
			break;
		case 0x35:
			printf( "DEC (HL)" );
			DEC( getByteAt( *reg->hl ), reg->f );
			break;
		case 0x36:
			printf( "LD (HL),+%X", readNextByte() );
			LD( getByteAt( *reg->hl ), getNextByte() );
			break;
		case 0x37:
			printf( "SCF" );
			SCF( reg->f );
			break;
		case 0x38:
			printf( "JR C,%d", (int8_t)readNextByte() );
			JRC( reg->pc, getNextByte(), reg->f );
			break;
		case 0x39:
			printf("ADD HL,SP");
			ADD16( reg->hl, reg->sp, reg->f );
			break;
		case 0x3A:
			printf( "LD A,(%X)", readNextWord() );	
			LD( reg->a, getByteAt( *(getNextWord()) ) );
			break;
		case 0x3B:
			printf("DEC SP");
			DEC16( reg->sp );
			break;
		case 0x3C:
			printf( "INC A" );
			INC( reg->a, reg->f );
			break;
		case 0x3D:
			printf( "DEC A" );
			DEC( reg->a, reg->f );
			break;
		case 0x3E:
			printf( "LD A,+%X", readNextByte() );
			LD( reg->a, getNextByte() );
			break;
		case 0x3F:
			printf( "CCF" );
			CCF( reg->f );
			break;
		case 0x40:
			printf( "LD B,B" );
			LD( reg->b, reg->b );
			break;
		case 0x41:
			printf( "LD B,C" );
			LD( reg->b, reg->c );
			break;
		case 0x42:
			printf( "LD B,D" );
			LD( reg->b, reg->d );
			break;
		case 0x43:
			printf( "LD B,E" );
			LD( reg->b, reg->e );
			break;
		case 0x44:
			printf( "LD B,H" );
			LD( reg->b, reg->h );
			break;
		case 0x45:
			printf( "LD B,L" );
			LD( reg->b, reg->l );
			break;
		case 0x46:
			printf( "LD B,(HL)" );
			LD( reg->b, getByteAt( *reg->hl ) );
			break;
		case 0x47:
			printf( "LD B,A");
			LD( reg->b, reg->a );
			break;
		case 0x48:
			printf( "LD C,B");
			LD( reg->c, reg->b );
			break;
		case 0x49:
			printf( "LD C,C" );
			LD( reg->c, reg->c );
			break;
		case 0x4A:
			printf( "LD C,D" );
			LD( reg->c, reg->d );
			break;
		case 0x4B:
			printf( "LD C,E" );
			LD( reg->c, reg->e );
			break;
		case 0x4C:
			printf( "LD C,H" );
			LD( reg->c, reg->h );
			break;
		case 0x4D:
			printf( "LD C,L" );
			LD( reg->c, reg->l );
			break;
		case 0x4E:
			printf( "LD C,(HL)" );
			LD( reg->c, getByteAt( *reg->hl ) );
			break;
		case 0x4F:
			printf( "LD C,A" );
			LD( reg->c, reg->a );
			break;
		case 0x50:
			printf( "LD D,B" );
			LD( reg->d, reg->b );
			break;
		case 0x51:
			printf( "LD D,C" );
			LD( reg->d, reg->c );
			break;
		case 0x52:
			printf( "LD D,D" );
			LD( reg->d, reg->d );
			break;
		case 0x53:
			printf( "LD D,E" );
			LD( reg->d, reg->e );
			break;
		case 0x54:
			printf( "LD D,H" );
			LD( reg->d, reg->h );
			break;
		case 0x55:
			printf( "LD D,L" );
			LD( reg->d, reg->l );
			break;
		case 0x56:
			printf( "LD D,(HL)" );
			LD( reg->d, getByteAt( *reg->hl ) );
			break;
		case 0x57:
			printf( "LD D,A" );
			LD( reg->d, reg->a );
			break;
		case 0x58:
			printf( "LD E,B" );
			LD( reg->e, reg->b );
			break;
		case 0x59:
			printf( "LD E,C" );
			LD( reg->e, reg->c );
			break;
		case 0x5A:
			printf( "LD E,D" );
			LD( reg->e, reg->d );
			break;
		case 0x5B:
			printf( "LD E,E" );
			LD( reg->e, reg->e );
			break;
		case 0x5C:
			printf( "LD E,H" );
			LD( reg->e, reg->h );
			break;
		case 0x5D:
			printf( "LD E,L" );
			LD( reg->e, reg->l );
			break;
		case 0x5E:
			printf( "LD E,(HL)" );
			LD( reg->e, getByteAt( *reg->hl ) );
			break;
		case 0x5F:
			printf( "LD E,A" );
			LD( reg->e, reg->a );
			break;
		case 0x60:
			printf( "LD H,B" );
			LD( reg->h, reg->b );
			break;
		case 0x61:
			printf( "LD H,C" );
			LD( reg->h, reg->c );
			break;
		case 0x62:
			printf( "LD H,D" );
			LD( reg->h, reg->d );
			break;
		case 0x63:
			printf( "LD H,E" );
			LD( reg->h, reg->e );
			break;
		case 0x64:
			printf( "LD H,H" );
			LD( reg->h, reg->h );
			break;
		case 0x65:
			printf( "LD H,L" );
			LD( reg->h, reg->l );
			break;
		case 0x66:
			printf( "LD H,(HL)" );
			LD( reg->h, getByteAt( *reg->hl ) );
			break; 
		case 0x67:
			printf( "LD H,A" );
			LD( reg->h, reg->a );
			break;
		case 0x68:
			printf( "LD L,B" );
			LD( reg->l, reg->b );
			break;
		case 0x69:
			printf( "LD L,C" );
			LD( reg->l, reg->c );
			break;
		case 0x6A:
			printf( "LD L,D");
			LD( reg->l, reg->d );
			break;
		case 0x6B:
			printf( "LD L,E");
			LD( reg->l, reg->e );
			break;
		case 0x6C:
			printf( "LD L,H" );
			LD( reg->l, reg->h );
			break;
		case 0x6D:
			printf( "LD L,L" );
			LD( reg->l, reg->l );
			break;
		case 0x6E:
			printf( "LD L,(HL)" );
			LD( reg->l, getByteAt( *reg->hl ) );
			break;
		case 0x6F:
			printf( "LD L,A" );
			LD( reg->l, reg->a );
			break;
		case 0x70:
			printf( "(HL),B");
			LD( getByteAt( *reg->hl ) ,reg->b );
			break;
		case 0x71:
			printf( "(HL),C");
			LD( getByteAt( *reg->hl ), reg->c );
			break;
		case 0x72:
			printf( "(HL),D");
			LD( getByteAt( *reg->hl ), reg->d );
			break;
		case 0x73:
			printf( "(HL),E");
			LD( getByteAt( *reg->hl ), reg->e );
			break;
		case 0x74:
			printf( "(HL),H");
			LD( getByteAt( *reg->hl ), reg->h );
			break;
		case 0x75:
			printf( "(HL),L");
			LD( getByteAt( *reg->hl ), reg->l );
			break;/*
		case 0x76:
			printf( "HALT" );
			break;*/
		case 0x77:
			printf( "LD (HL),A");
			LD( getByteAt( *reg->hl ) ,reg->a );
			break;
		case 0x78:
			printf( "LD A,B");
			LD( reg->a, reg->b );
			break;
		case 0x79:
			printf( "LD A,C");
			LD( reg->a, reg->c );
			break;
		case 0x7A:
			printf( "LD A,D" );
			LD( reg->a, reg->d );
			break;
		case 0x7B:
			printf( "LD A,E" );
			LD( reg->a, reg->e );
			break;
		case 0x7C:
			printf( "LD A,H" );
			LD( reg->a, reg->h );
			break;
		case 0x7D:
			printf( "LD A,L" );
			LD( reg->a, reg->l );
			break;
		case 0x7E:
			printf( "LD A,(HL)" );
			LD( reg->a, getByteAt( *reg->hl ) );
			break;
		case 0x7F:
			printf( "LD A,A" );
			LD( reg->a, reg->a );
			break;
		case 0x80:
			printf( "ADD A,B" );
			ADD( reg->a, reg->b, reg->f );
			break;
		case 0x81:
			printf( "ADD A,C" );
			ADD( reg->a, reg->c, reg->f );
			break;
		case 0x82:
			printf( "ADD A,D" );
			ADD( reg->a, reg->d, reg->f );
			break;
		case 0x83:
			printf( "ADD A,E" );
			ADD( reg->a, reg->e, reg->f );
			break;
		case 0x84:
			printf( "ADD A,H" );
			ADD( reg->a, reg->h, reg->f );
			break;
		case 0x85:
			printf( "ADD A,L" );
			ADD( reg->a, reg->l, reg->f );
			break;
		case 0x86:
			printf( "ADD (HL)" );
			ADD( reg->a, getByteAt( *reg->hl ), reg->f );
			break;
		case 0x87:
			printf( "ADD A,A" );
			ADD( reg->a, reg->a, reg->f );
			break;
		case 0x88:
			printf( "ADC A,B" );
			ADC( reg->a, reg->b, reg->f );
			break;
		case 0x89:
			printf( "ADC A,C" );
			ADC( reg->a, reg->c, reg->f );
			break;
		case 0x8A:
			printf( "ADC A,D" );
			ADC( reg->a, reg->d, reg->f );
			break;
		case 0x8B:
			printf( "ADC A,E" );
			ADC( reg->a, reg->e, reg->f );
			break;
		case 0x8C:
			printf( "ADC A,H" );
			ADC( reg->a, reg->h, reg->f );
			break;
		case 0x8D:
			printf( "ADC A,L" );
			ADC( reg->a, reg->l, reg->f );
			break;
		case 0x8E:
			printf( "ADC A,(HL)" );
			ADC( reg->a, getByteAt( *reg->hl ), reg->f );
			break;
		case 0x8F:
			printf( "ADC A,A" );
			ADC( reg->a, reg->a, reg->f );
			break;
		case 0x90:
			printf( "SUB B" );
			SUB( reg->a, reg->b, reg->f );
			break;
		case 0x91:
			printf( "SUB C" );
			SUB( reg->a, reg->c, reg->f );
			break;
		case 0x92:
			printf( "SUB D" );
			SUB( reg->a, reg->d, reg->f );
			break;
		case 0x93:
			printf( "SUB E" );
			SUB( reg->a, reg->e, reg->f );
			break;
		case 0x94:
			printf( "SUB H" );
			SUB( reg->a, reg->h, reg->f );
			break;
		case 0x95:
			printf( "SUB L" );
			SUB( reg->a, reg->l, reg->f );
			break;
		case 0x96:
			printf( "SUB (HL)" );
			SUB( reg->a, getByteAt( *reg->hl ), reg->f );
			break;
		case 0x97:
			printf( "SUB A" );
			SUB( reg->a, reg->a, reg->f );
			break;
		case 0x98:
			printf( "SBC A,B" );
			SBC( reg->a, reg->b, reg->f );
			break;
		case 0x99:
			printf( "SBC A,C" );
			SBC( reg->a, reg->c, reg->f );
			break;
		case 0x9A:
			printf( "SBC A,D" );
			SBC( reg->a, reg->d, reg->f );
			break;
		case 0x9B:
			printf( "SBC A,E" );
			SBC( reg->a, reg->e, reg->f );
			break;
		case 0x9C:
			printf( "SBC A,H" );
			SBC( reg->a, reg->h, reg->f );
			break;
		case 0x9D:
			printf( "SBC A,L" );
			SBC( reg->a, reg->l, reg->f );
			break;
		case 0x9E:
			printf( "SBC A,(HL)" );
			SBC( reg->a, getByteAt( *reg->hl ), reg->f );
			break;
		case 0x9F:
			printf( "SBC A,A" );
			SBC( reg->a, reg->a, reg->f );
			break;
		case 0xA0:
			printf( "AND B" );
			AND( reg->a, reg->b, reg->f );
			break;
		case 0xA1:
			printf( "AND C" );
			AND( reg->a, reg->c, reg->f );
			break;
		case 0xA2:
			printf( "AND D" );
			AND( reg->a, reg->d, reg->f );
			break;
		case 0xA3:
			printf( "AND E" );
			AND( reg->a, reg->e, reg->f );
			break;
		case 0xA4:
			printf( "AND H" );
			AND( reg->a, reg->h, reg->f );
			break;
		case 0xA5:
			printf( "AND L" );
			AND( reg->a, reg->l, reg->f );
			break;
		case 0xA6:
			printf( "AND (HL)" );
			AND( reg->a, getByteAt( *reg->hl ), reg->f );
			break;
		case 0xA7:
			printf( "AND A" );
			AND( reg->a, reg->a, reg->f );
			break;
		case 0xA8:
			printf( "XOR B" );
			XOR( reg->a, reg->b, reg->f );
			break;
		case 0xA9:
			printf( "XOR C" );
			XOR( reg->a, reg->c, reg->f );
			break;
		case 0xAA:
			printf( "XOR D" );
			XOR( reg->a, reg->d, reg->f );
			break;
		case 0xAB:
			printf( "XOR E" );
			XOR( reg->a, reg->e, reg->f );
			break;
		case 0xAC:
			printf( "XOR H" );
			XOR( reg->a, reg->h, reg->f );
			break;
		case 0xAD:
			printf( "XOR L" );
			XOR( reg->a, reg->l, reg->f );
			break;
		case 0xAE:
			printf( "XOR (HL)" );
			XOR( reg->a, getByteAt( *reg->hl ), reg->f );
			break;
		case 0xAF:
			printf( "XOR A");
			XOR( reg->a, reg->a, reg->f );
			break;
		case 0xB0:
			printf( "OR B" );
			OR( reg->a, reg->b, reg->f );
			break;
		case 0xB1:
			printf( "OR C" );
			OR( reg->a, reg->c, reg->f );
			break;
		case 0xB2:
			printf( "OR D" );
			OR( reg->a, reg->d, reg->f );
			break;
		case 0xB3:
			printf( "OR E" );
			OR( reg->a, reg->e, reg->f );
			break;
		case 0xB4:
			printf( "OR H" );
			OR( reg->a, reg->h, reg->f );
			break;
		case 0xB5:	
			printf( "OR L" );
			OR( reg->a, reg->l, reg->f );
			break;
		case 0xB6:
			printf( "OR(HL)" );
			OR( reg->a, getByteAt( *reg->hl ), reg->f );
			break;
		case 0xB7:
			printf( "OR A" );
			OR( reg->a, reg->a, reg->f );
			break;
		case 0xB8:
			printf( "CP B" );
			CP( reg->a, reg->b, reg->f );
			break;
		case 0xB9:
			printf( "CP C" );
			CP( reg->a, reg->c, reg->f );
			break;
		case 0xBA:
			printf( "CP D" );
			CP( reg->a, reg->d, reg->f );
			break;
		case 0xBB:
			printf( "CP E" );
			CP( reg->a, reg->e, reg->f );
			break;
		case 0xBC:
			printf( "CP H");	
			CP( reg->a, reg->h, reg->f );
			break;
		case 0xBD:
			printf( "CP L");
			CP( reg->a, reg->l, reg->f );
			break;
		case 0xBE:
			printf( "CP (HL)" );
			CP( reg->a, getByteAt( *reg->hl ), reg->f );
			break;
		case 0xBF:
			printf( "CP A" );
			CP( reg->a, reg->a, reg->f );
			break;
		case 0xC0:
			printf( "RET NZ" );
			RETNZ( reg->pc, getWordAt( reg->sp ), reg->sp, reg->f );
			break;
		case 0xC1:
			printf( "POP BC");
			POP( getWordAt( reg->sp ) ,reg->sp, reg->bc );
			break;
		case 0xC2:
			printf( "JP NZ, %X", readNextWord() );
			JPNZ( reg->pc, getNextWord(), reg->f );
			break;
		case 0xC3:
			printf( "JP %X", readNextWord() );
			JP( reg->pc, getNextWord() );
			break;
		case 0xC4:
			printf( "CALLNZ %X", readNextWord() );
			CALLNZ( getNextWord(), getWordAt( reg->sp ), reg->sp, reg->pc, reg->f );
			break;
		case 0xC5:
			printf( "PUSH BC");
			PUSH( getWordAt( reg->sp ), reg->sp, reg->bc );
			break;
		case 0xC6:
			printf( "ADD A,%X", readNextByte() );
			ADD( reg->a, getNextByte(), reg->f );
			break;
		case 0xC7:
			printf( "RST 00H" );
			RST( 0x00, getWordAt( reg->sp ), reg->sp, reg->pc );
			break;
		case 0xC8:
			printf( "RET Z" );
			RETZ( reg->pc, getWordAt( reg->sp ), reg->sp, reg->f );
			break;
		case 0xC9:
			printf( "RET");
			RET( reg->pc, getWordAt( reg->sp ), reg->sp );
			break;
		case 0xCA:
			printf( "JP Z,%X", readNextWord() );
			JPZ( reg->pc, getNextWord(), reg->f );
			break;
		case 0xCB:
			switch( *( getNextByte() ) ){
				case 0x00:
					printf( "RLC B" );
					RLC( reg->b, reg->f );
					break;
				case 0x01:
					printf( "RLC C" );
					RLC( reg->c, reg->f );
					break;
				case 0x02:
					printf( "RLC D" );
					RLC( reg->d, reg->f );
					break;
				case 0x03:
					printf( "RLC E" );
					RLC( reg->e, reg->f );
					break;
				case 0x04:
					printf( "RLC H" );
					RLC( reg->h, reg->f );
					break;
				case 0x05:
					printf( "RLC L" );
					RLC( reg->l, reg->f );
					break;
				case 0x06:
					printf( "RLC (HL)" );
					RLC( getByteAt( *reg->hl ), reg->f );
					break;
				case 0x07:
					printf( "RLC A" );
					RLC( reg->a, reg->f );
					break;
				case 0x08:
					break;
				case 0x09:
					break;
				case 0x0A:
					break;	
				case 0x0B:
					break;
				case 0x0C:
					break;
				case 0x0D:
					break;
				case 0x0E:
					break;
				case 0x0F:
					break;
				case 0x10:
					break;
				case 0x11:
					break;
				case 0x12:
					break;
				case 0x13:
					break;
				case 0x14:
					break;
				case 0x15:
					break;
				case 0x16:
					break;
				case 0x17:
					break;
				case 0x18:
					break;
				case 0x19:
					break;
				case 0x1A:
					break;	
				case 0x1B:
					break;
				case 0x1C:
					break;
				case 0x1D:
					break;
				case 0x1E:
					break;
				case 0x1F:
					break;
				case 0x20:
					break;
				case 0x21:
					break;
				case 0x22:
					break;
				case 0x23:
					break;
				case 0x24:
					break;
				case 0x25:
					break;
				case 0x26:
					break;
				case 0x27:
					break;
				case 0x28:
					break;
				case 0x29:
					break;
				case 0x2A:
					break;	
				case 0x2B:
					break;
				case 0x2C:
					break;
				case 0x2D:
					break;
				case 0x2E:
					break;
				case 0x2F:
					break;
				case 0x30:
					break;
				case 0x31:
					break;
				case 0x32:
					break;
				case 0x33:
					break;
				case 0x34:
					break;
				case 0x35:
					break;
				case 0x36:
					break;
				case 0x37:
					break;
				case 0x38:
					break;
				case 0x39:
					break;
				case 0x3A:
					break;	
				case 0x3B:
					break;
				case 0x3C:
					break;
				case 0x3D:
					break;
				case 0x3E:
					break;
				case 0x3F:
					break;
				case 0x40:
					break;
				case 0x41:
					break;
				case 0x42:
					break;
				case 0x43:
					break;
				case 0x44:
					break;
				case 0x45:
					break;
				case 0x46:
					printf( "BIT 0" );
					BIT( 0, getByteAt( *reg->hl ), reg->f );
					break;	
				case 0x47:
					break;
				case 0x48:
					break;
				case 0x49:
					break;
				case 0x4A:
					break;	
				case 0x4B:
					break;
				case 0x4C:
					break;
				case 0x4D:
					break;
				case 0x4E:
					printf( "BIT 1" );
					BIT( 1, getByteAt( *reg->hl ), reg->f );
					break;
				case 0x4F:
					break;
				case 0x50:
					break;
				case 0x51:
					break;
				case 0x52:
					break;
				case 0x53:
					break;
				case 0x54:
					break;
				case 0x55:
					break;
				case 0x56:
					printf( "BIT 2" );
					BIT( 2, getByteAt( *reg->hl ), reg->f );
					break;
				case 0x57:
					break;
				case 0x58:
					break;
				case 0x59:
					break;
				case 0x5A:
					break;	
				case 0x5B:
					break;
				case 0x5C:
					break;
				case 0x5D:
					break;
				case 0x5E:
					printf( "BIT 3" );
					BIT( 3, getByteAt( *reg->hl ), reg->f );
					break;
				case 0x5F:
					break;
				case 0x60:
					printf( "BIT 4" );
					BIT( 4, reg->b, reg->f );
					break;
				case 0x61:
					printf( "BIT 4" );
					BIT( 4, reg->c, reg->f );
					break;
				case 0x62:
					printf( "BIT 4" );
					BIT( 4, reg->d, reg->f );
					break;
				case 0x63:
					printf( "BIT 4" );
					BIT( 4, reg->e, reg->f );
					break;
				case 0x64:
					printf( "BIT 4" );
					BIT( 4, reg->h, reg->f );
					break;
				case 0x65:
					printf( "BIT 4" );
					BIT( 4, reg->l, reg->f );
					break;
				case 0x66:
					printf( "BIT 4" );
					BIT( 4, getByteAt( *reg->hl ), reg->f );
					break;
				case 0x67:
					printf( "BIT 4" );
					BIT( 4, reg->a, reg->f );
					break;
				case 0x6E:
					printf( "BIT 5, (HL)" );
					BIT( 5, getByteAt( *reg->hl ), reg->f );
					break;
				case 0x76:
					printf( "BIT 6, (HL)" );
					BIT( 6, getByteAt( *reg->hl ), reg->f );
					break;
				case 0x7E:
					printf( "BIT 7, (HL)" );
					BIT( 7, getByteAt( *reg->hl ), reg->f );
					break;
				case 0x86:
					printf( "RES 0, (HL)" );
					RES( 0, getByteAt( *reg->hl ) );
					break;	
				case 0x8E:
					printf( "RES 1, (HL)" );
					RES( 1, getByteAt( *reg->hl ) );
					break;
				case 0x96:
					printf( "RES 2, (HL)" );
					RES( 2, getByteAt( *reg->hl ) );
					break;
				case 0x9E:
					printf( "RES 3, (HL)" );
					RES( 3, getByteAt( *reg->hl ) );
					break;
				case 0xA6:
					printf( "RES 4, (HL)" );
					RES( 4, getByteAt( *reg->hl ) );
					break;
				case 0xAE:
					printf( "RES 5, (HL)" );
					RES( 5, getByteAt( *reg->hl ) );
					break;
				case 0xB6:
					printf( "RES 6, (HL)" );
					RES( 6, getByteAt( *reg->hl ) );
					break;
				case 0xBC:	
					printf( "RES 7, H" );
					RES( 7, reg->h );
					break;	
				case 0xBE:
					printf( "RES 7, (HL)" );
					RES( 7, getByteAt( *reg->hl ) );
					break;
				case 0xC6:
					printf( "SET 0, (HL)" );
					SET( 0, getByteAt( *reg->hl ) );
					break;	
				case 0xCE:
					printf( "SET 1, (HL)" );
					SET( 1, getByteAt( *reg->hl ) );
					break;
				case 0xD6:
					printf( "SET 2, (HL)" );	
					SET( 2, getByteAt( *reg->hl ) );
					break;
				case 0xDE:
					printf( "SET 3, (HL)" );
					SET( 3, getByteAt( *reg->hl ) );
					break;
				case 0xE6:
					printf( "SET 4, (HL)" );
					SET( 4, getByteAt( *reg->hl ) );
					break;
				case 0xEE:
					printf( "SET 5, (HL)" );
					SET( 5, getByteAt( *reg->hl ) );
					break;
				case 0xF6:
					printf( "SET 6, (HL)" );
					SET( 6, getByteAt( *reg->hl ) );
					break;
				case 0xFD:
					printf( "SET 7, L" );
					SET( 7, reg->l );
					break;
				case 0xFE:
					printf( "SET 7, (HL)" );
					SET( 7, getByteAt( *reg->hl ) );
					break;
				default:
					break;
			}
			break;
		case 0xCC:
			printf( "CALL Z,%X", readNextWord() );
			CALLZ( getNextWord(), getWordAt( reg->sp ), reg->sp, reg->pc, reg->f );
			break;
		case 0xCD:
			printf( "CALL %X", readNextWord() );
			CALL( getNextWord(), getWordAt( reg->sp ), reg->sp, reg->pc );
			break;
		case 0xCE:
			printf( "ADC A,%X", readNextByte() );
			ADC( reg->a, getNextByte(), reg->f );
			break;
		case 0xCF:
			printf( "RST 08H" );
			RST( 0x08, getWordAt( reg->sp ), reg->sp, reg->pc );
			break;
		case 0xD0:
			printf( "RET NC" );
			RETNC( reg->pc, getWordAt( reg->sp ), reg->sp, reg->f );
			break;
		case 0xD1:
			printf( "POP DE");
			POP( getWordAt( reg->sp ) ,reg->sp, reg->de );
			break;
		case 0xD2:
			printf( "JP NC,%X", readNextWord() );
			JPNC( reg->pc, getNextWord(), reg->f );
			break;
		case 0xD3:
			printf("OUT (+%X),A", readNextByte() );
			OUTA( &IOport[ *( getNextByte() ) ], reg->a );
			break;
		case 0xD4:
			printf( "CALL NC,%X", readNextWord() );
			CALLNC( getNextWord(), getWordAt( reg->sp ), reg->sp, reg->pc, reg->f  );
			break;
		case 0xD5:
			printf( "PUSH DE");
			PUSH( getWordAt( reg->sp ), reg->sp, reg->de );
			break;
		case 0xD6:
			printf( "SUB %X", readNextByte() );
			SUB( reg->a, getNextByte(), reg->f );
			break;
		case 0xD7:
			printf( "RST 10" );
			RST( 0x10, getWordAt( reg->sp ), reg->sp, reg->pc );
			break;
		case 0xD8:
			printf( "RET C" );
			RETC( reg->pc, getWordAt( reg->sp ), reg->sp, reg->f );
			break;
		case 0xD9:
			printf( "EXX" );
			EXX( reg->bc, reg->de, reg->hl, reg->altbc, reg->altde, reg->althl );
			break;
		case 0xDA:
			printf( "JP C,%X", readNextWord() );
			JPC( reg->pc, getNextWord(), reg->f );
			break;
		case 0xDB:
			printf( "IN A,(C)" );
			INA( reg->a, IOport[0xFE] );
			break;
		case 0xDC:
			printf( "CALL C,%X", readNextWord() );
			CALLC( getNextWord(), getWordAt( reg->sp ), reg->sp, reg->pc, reg->f  );
			break;
		case 0xDD: /* IX registers */
			switch( *( getNextByte() ) ){
			case 0x21:
				printf( "LD IX,+%X", readNextWord() );
				LD16( reg->ix, getNextWord() );
				break;
			case 0x22:
				printf( "LD (%X),IX", readNextWord() );
				LD16( getWordAt( getNextWord() ), reg->ix );	
				break;
			case 0x23:
				printf( "INC IX" );
				INC16( reg->iy );
				break;
			case 0x2A:
				printf( "LD IX(%X)", readNextWord() );
				LD16( reg->ix, getWordAt( getNextWord() ) );
				break;
			case 0x2B:
				printf( "DEC IX" );
				DEC16( reg->ix );
				break;
			case 0x34:
				printf( "INC IX+%X",(int8_t) readNextByte() );
				INC( getByteAt( *reg->ix +(int8_t) *( getNextByte() ) ), reg->f );
				break;
			case 0x35:
				printf( "DEC IX +%X",(int8_t) readNextByte() );
				DEC( getByteAt( *reg->ix +(int8_t) *( getNextByte() ) ), reg->f );
				break;
			case 0x36:
				printf( "LD (IX+%X),+%X",(int8_t) readNextByte(), readNextByte() + 1 ); // BROKEN second param is incorrect
				LD( getByteAt( *reg->ix +(int8_t) *( getNextByte() ) ), getNextByte() );
				break;
			case 0x46:
				printf( "LD B,(IX+%X)",(int8_t) readNextByte() );
				LD( reg->b, getByteAt( *reg->ix +(int8_t) *(getNextByte()) ) ); 
				break;
			case 0x4E:
				printf( "LD C,(IX+%X)",(int8_t) readNextByte() );
				LD( reg->c, getByteAt( *reg->ix +(int8_t) *(getNextByte() ) ) ); 
				break;
			case 0x5E:
				printf( "LD D,(IX+%X)",(int8_t) readNextByte() );
				LD( reg->d, getByteAt( *reg->ix +(int8_t) *(getNextByte() ) ) ); 
				break;
			case 0x56:
				printf( "LD E,(IX+%X)",(int8_t) readNextByte() );
				LD( reg->e, getByteAt( *reg->ix + (int8_t)*(getNextByte() ) ) ); 
				break;
			case 0x66:
				printf( "LD H,(IX+%X)",(int8_t) readNextByte() );
				LD( reg->h, getByteAt( *reg->ix +(int8_t) *(getNextByte() ) ) ); 
				break;
			case 0x6E:
				printf( "LD L,(IX+%X)",(int8_t) readNextByte() );
				LD( reg->l, getByteAt( *reg->ix +(int8_t) *( getNextByte() ) ) ); 
				break;
			case 0x70:
				printf( "LD (IX+%X),B",(int8_t) readNextByte() );
				LD( getByteAt( *reg->ix + (int8_t)*( getNextByte() ) ), reg->b );
				break;
			case 0x71:
				printf( "LD (IX+%X),C", (int8_t)readNextByte() );
				LD( getByteAt( *reg->ix + (int8_t)*( getNextByte() ) ), reg->c );				
				break;
			case 0x72:
				printf( "LD (IX+%X),D", (int8_t)readNextByte() );
				LD( getByteAt( *reg->ix + (int8_t)*( getNextByte() ) ), reg->d );
				break;
			case 0x73:
				printf( "LD (IX+%X),E", (int8_t)readNextByte() );
				LD( getByteAt( *reg->ix + (int8_t)*( getNextByte() ) ), reg->e );
				break;
			case 0x74:
				printf( "LD (IX+%X),H", (int8_t)readNextByte() );
				LD( getByteAt( *reg->ix + (int8_t)*( getNextByte() ) ), reg->h );
				break;
			case 0x75:
				printf( "LD (IX+%X),L", (int8_t)readNextByte() );
				LD( getByteAt( *reg->ix + (int8_t)*( getNextByte() ) ), reg->l );
				break;
			case 0x77:
				printf( "LD (IX+%X),A", (int8_t)readNextByte() );
				LD( getByteAt( *reg->ix + (int8_t)*( getNextByte() ) ), reg->a );
				break;
			case 0x7E:
				printf( "LD A,(IX+%X)", (int8_t)readNextByte() );
				LD( reg->a, getByteAt( *reg->ix + (int8_t)*( getNextByte() ) ) ); 
				break;
			case 0x86:
				printf( "ADD (IX+%X)", (int8_t)readNextByte() );
				ADD( reg->a, getByteAt( *reg->ix + (int8_t)*( getNextByte() ) ), reg->f );
				break;
			case 0x8E:
				printf( "ADC (IX+%X)", (int8_t)readNextByte() );
				ADC( reg->a, getByteAt( *reg->ix + (int8_t)*( getNextByte() ) ), reg->f );
				break;
			case 0x96:
				printf( "SUB (IX+%X)", (int8_t)readNextByte() );
				SUB( reg->a, getByteAt( *reg->ix + (int8_t)*( getNextByte() ) ), reg->f );
				break;
			case 0xA6:
				printf( "AND A (IX+%X)",(int8_t)readNextByte() );
				AND( reg->a, getByteAt( *reg->ix + (int8_t)*( getNextByte() ) ), reg->f );
				break;
			case 0xAE:
				printf( "XOR (IX+%X)", (int8_t)readNextByte() );
				XOR( reg->a, getByteAt( *reg->ix + (int8_t)*( getNextByte() ) ), reg->f );
				break;
			case 0xB6:
				printf( "OR A,(IX+%X)", (int8_t)readNextByte() );
				OR( reg->a, getByteAt( *reg->ix + (int8_t)*( getNextByte() ) ), reg->f );
				break;
			case 0xBE:
				printf( "CP (IX+%X)",(int8_t) readNextByte() );
				CP( reg->a, getByteAt( *reg->ix +(int8_t) *( getNextByte() ) ) ,reg->f );
				break;
			case 0xCB:
				offset = *( getNextByte() );
				switch( *( getNextByte() ) ){	
					case 0x06:
						printf( "RLC(IX+%X)", offset );
						RLC( getByteAt( *reg->ix + (int8_t) *( getNextByte() ) ), reg->f );
						break;	
					case 0x08:
						break;
					case 0x09:
						break;
					case 0x0A:
						break;
					case 0x0B:
						break;
					case 0x0C:
						break;
					case 0x0D:
						break;	
					case 0x0E:
						break;
					case 0x0F:
						break;
					case 0x10:
						break;
					case 0x11:
						break;
					case 0x12:
						break;
					case 0x13:
						break;
					case 0x14:
						break;
					case 0x15:
						break;	
					case 0x16:
						break;
					case 0x17:
						break;
					case 0x18:
						break;
					case 0x19:
						break;
					case 0x1A:
						break;
					case 0x1E:
						break;
					case 0x26:
						break;
					case 0x2E:
						break;
					case 0x3E:	
						break;
					case 0x46:
						printf( "BIT 0, (IX+%X)", (int8_t)offset );
						BIT( 0, getByteAt( *reg->ix + (int8_t)offset ), reg->f );
						break;	
					case 0x4E:
						printf( "BIT 1, (IX+%X)", (int8_t)offset );
						BIT( 1, getByteAt( *reg->ix + (int8_t)offset ), reg->f );
						break;
					case 0x56:
						printf( "BIT 2, (IX+%X)", (int8_t)offset );
						BIT( 2, getByteAt( *reg->ix + (int8_t)offset ), reg->f );
						break;
					case 0x5E:
					        printf( "BIT 3, (IX+%X)", (int8_t)offset );
						BIT( 3, getByteAt( *reg->ix + (int8_t)offset ), reg->f );
						break;
					case 0x66:
						printf( "BIT 4, (IX+%X)", (int8_t)offset );
						BIT( 4, getByteAt( *reg->ix + (int8_t)offset ), reg->f );
						break;
					case 0x6E:
						printf( "BIT 5, (IX+%X)", (int8_t)offset );
						BIT( 5, getByteAt( *reg->ix + (int8_t)offset ), reg->f );
						break;
					case 0x76:
						printf( "BIT 6, (IX+%X)", (int8_t)offset );
						BIT( 6, getByteAt( *reg->ix + (int8_t)offset ), reg->f );
						break;
					case 0x7E:
						printf( "BIT 7, (IX+%X)", (int8_t)offset );
						BIT( 7, getByteAt( *reg->ix + (int8_t)offset ), reg->f );
						break;
					case 0x86:
						printf( "RES 0, (IX+%X)", (int8_t)offset );
						RES( 0, getByteAt( *reg->ix + (int8_t)offset ) );
						break;	
					case 0x8E:
						printf( "RES 1, (IX+%X)", (int8_t)offset );
						RES( 1, getByteAt( *reg->ix + (int8_t)offset ) );
						break;
					case 0x96:
						printf( "RES 2, (IX+%X)", (int8_t)offset );
						RES( 2, getByteAt( *reg->ix + (int8_t)offset ) );
						break;
					case 0x9E:
					        printf( "RES 3, (IX+%X)", (int8_t)offset );
						RES( 3, getByteAt( *reg->ix + (int8_t)offset ) );
						break;
					case 0xA6:
						printf( "RES 4, (IX+%X)", (int8_t)offset );
						RES( 4, getByteAt( *reg->ix + (int8_t)offset ) );
						break;
					case 0xAE:
						printf( "RES 5, (IX+%X)", (int8_t)offset );
						RES( 5, getByteAt( *reg->ix + (int8_t)offset ) );
						break;
					case 0xB6:
						printf( "RES 6, (IX+%X)", (int8_t)offset );
						RES( 6, getByteAt( *reg->ix + (int8_t)offset ) );
						break;	
					case 0xBE:
						printf( "RES 7, (IX+%X)", (int8_t)offset );
						RES( 7, getByteAt( *reg->ix + (int8_t)offset ) );
						break;
					case 0xC6:
						printf( "SET 0, (IX+%X)", (int8_t)offset );
						SET( 0, getByteAt( *reg->ix + (int8_t)offset ) );
						break;	
					case 0xCE:
						printf( "SET 1, (IX+%X)", (int8_t)offset );
						SET( 1, getByteAt( *reg->ix + (int8_t)offset ) );
						break;
					case 0xD6:
						printf( "SET 2, (IX+%X)", (int8_t)offset );	
						SET( 2, getByteAt( *reg->ix + (int8_t)offset ) );
						break;
					case 0xDE:
					        printf( "SET 3, (IX+%X)", (int8_t)offset );
						SET( 3, getByteAt( *reg->ix + (int8_t)offset ) );
						break;
					case 0xE6:
						printf( "SET 4, (IX+%X)", (int8_t)offset );
						SET( 4, getByteAt( *reg->ix + (int8_t)offset ) );
						break;
					case 0xEE:
						printf( "SET 5, (IX+%X)", (int8_t)offset );
						SET( 5, getByteAt( *reg->ix + (int8_t)offset ) );
						break;
					case 0xF6:
						printf( "SET 6, (IX+%X)", (int8_t)offset );
						SET( 6, getByteAt( *reg->ix + (int8_t)offset ) );
						break;	
					case 0xFE:
						printf( "SET 7, (IX+%X)", (int8_t)offset );
						SET( 7, getByteAt( *reg->ix + (int8_t)offset ) );
						break;
					default:
						break;
				}
				break;
			case 0xE1:
				printf( "POP IX");
				POP( getWordAt( reg->sp ) ,reg->sp, reg->ix );
				break;
			case 0xE3:
				printf( "EX (SP),IX" );
				EX( getWordAt( reg->sp ), reg->ix );
				break;
			case 0xE5:
				printf( "PUSH IX" );
				PUSH( getWordAt( reg->sp ), reg->sp, reg->ix );
				break;
			case 0xE9:
				printf( "JP(IX)" );
				JP( reg->pc, getWordAt( reg->ix ) );
				break;
			case 0xF9:
				printf( "LD SP,IX" );
				LD16( reg->sp, reg->ix );
				break;
			default:
				break;
			}
			break;
		case 0xDE:
			printf( "SBC A,%X", readNextByte() );
			SBC( reg->a, getNextByte(), reg->f );
			break;
		case 0xDF:
			printf( "RST 18H" );
			RST( 0x18, getWordAt( reg->sp ), reg->sp, reg->pc );
			break;
		case 0xE0:
			printf( "RET PO,%X***MUST BE CHECKED", readNextWord() );
			RETPO( reg->pc, getWordAt( reg->sp ), reg->sp, reg->f );
			break;
		case 0xE1:
			printf( "POP HL");
			POP( getWordAt( reg->sp ) ,reg->sp, reg->hl );
			break;
		case 0xE2:
			printf( "JP PO,%X ****MUST BE CHECKED", readNextWord() );
			JPPO( reg->pc, getNextWord(), reg->f );
			break;
		case 0xE3:
			printf( "EX (SP),HL" );
			EX( getWordAt( reg->sp ), reg->hl );
			break;
		case 0xE4:
			printf( "CALL PO,%X***MUST BE CHECKED", readNextWord() );
			CALLPO( getNextWord(), getWordAt( reg->sp ), reg->sp, reg->pc, reg->f  );
			break;
		case 0xE5:
			printf( "PUSH HL");
			PUSH( getWordAt( reg->sp ), reg->sp, reg->hl );
			break;
		case 0xE6:
			printf("AND %X", readNextByte() );
			AND( reg->a, getNextByte(), reg->f );
			break;
		case 0xE7:
			printf( "RST 20H" );
			RST( 0x20, getWordAt( reg->sp ), reg->sp, reg->pc );
			break;
		case 0xE8:
			printf( "RET PE,%X", readNextWord() );
			RETPE( reg->pc, getWordAt( reg->sp ), reg->sp, reg->f  );
			break;
		case 0xE9:
			printf( "JP (HL)");
			JP( reg->pc, reg->hl );
			break;
		case 0xEA:
			printf( "JP PE,%X***Check this", readNextWord() );
			JPPE( reg->pc, getNextWord(), reg->f );
			break;
		case 0xEB:
			printf( "EX DE,HL");
			EX( reg->de, reg->hl );
			break;
		case 0xEC:
			printf( "CALL PE,%X***Check this", readNextWord() );
			CALLPE( getNextWord(), getWordAt( reg->sp ), reg->sp, reg->pc, reg->f  );
			break;
		case 0xED:
			switch( *( getNextByte() ) ){
			case 0x42:
				printf( "SBC HL,BC" );
				SBC16( reg->hl, reg->bc, reg->f );
				break;
			case 0x43:
				printf( "LD (%X),BC", readNextWord() );
				LD16( getWordAt( getNextWord() ) ,reg->bc );				
				break;
			case 0x46:
				printf( "IM 0" );
				IM0( reg->mode );
				break;
			case 0x47:
				printf( "LD I,A");
				LD( reg->i, reg->a );
				break;
			case 0x4B:
				printf( "LD BC,(%X)", readNextWord() );
				LD16( reg->bc, getWordAt( getNextWord() ) );
				break;
			case 0x4F:
				printf( "LD R,A");
				LD( reg->r, reg->a );
				break;
			case 0x52:
				printf( "SBC HL,DE");
				SBC16( reg->hl, reg->de, reg->f );
				break;
			case 0x53:
				printf( "LD (%X),DE", readNextWord() );
				LD16( getWordAt( getNextWord() ) ,reg->de );				
				break;
			case 0x56:
				printf( "IM 1" ); 
				IM1( reg->mode );
				break;
			case 0x57:
				printf( "LD A,I");
				LDAIR( reg->a, reg->i, reg->f, reg->iff2 );
				break;
			case 0x5B:
				printf( "LD DE,(%X)", readNextWord() );
				LD16( reg->de, getWordAt( getNextWord() ) );
				break;
			case 0x5E:
				printf( "IM 2" );
				IM2( reg->mode );
				break;
			case 0x5F:
				printf( "LD A,R");
				LDAIR( reg->a, reg->r, reg->f, reg->iff2 );
				break;
			case 0x62:
				printf( "SBC HL,HL" );
				SBC16( reg->hl, reg->hl, reg->f );
				break;
			case 0x72:
				printf( "SBC HL,SP" );
				SBC16( reg->hl, reg->sp, reg->f );
				break;
			case 0x73:
				printf( "LD(%X),SP", readNextWord() );
				LD16( getNextWord(), reg->sp );
				break;
			case 0x78:
				printf( "IN A,(C)" );
				IN( reg->a, IOport[0xFE], reg->f );
				break;
			case 0x7B:
				printf( "LD SP,(%X)", readNextWord() );
				LD16( reg->sp, getWordAt( getNextWord() ) );
				break;/*
			case 0xA0:
				//printf( "LDI");
				LDI();
				break;
			case 0xA1:
				//printf( "CPI");
				CPI();
				break;
			case 0xA8:
				//printf( "LLD");
				LDD();
				break;
			case 0xEDA9:
				//printf( "CPD");
				CPD();
			break;*/
			case 0xB0:
				printf( "LDIR");
				LDIR( getWordAt( reg->hl ), getWordAt( reg->de ), reg->hl, reg->de, reg->bc, reg->pc, reg->f );
				break;/*
			case 0xB1:
				//printf( "CPIR");
				CPIR();
				break;*/
			case 0xB8:
				printf( "LDDR");
				LDDR( getWordAt( reg->hl ), getWordAt( reg->de ), reg->hl, reg->de, reg->bc, reg->f );
				break;
			case 0xB9:
				//printf( "CPDR");
				//CPDR();
				break;
			default:
				break;
			}
		break;
		case 0xEE:
			printf( "XOR %X",readNextByte() );
			XOR( reg->a, getNextByte(), reg->f );
			break;
		case 0xEF:
			printf( "RST 28H" );
			RST( 0x28, getWordAt( reg->sp ), reg->sp, reg->pc );
			break;
		case 0xF0:
			printf( "RET P" );
			RETP( reg->pc, getWordAt( reg->sp ), reg->sp, reg->f  );
			break;
		case 0xF1:
			printf( "POP AF");
			POP( getWordAt( reg->sp ) ,reg->sp, reg->af );
			break;
		case 0xF2:
			printf( "JP P,%X***Check this", readNextWord() );
			JPP( reg->pc, getNextWord(), reg->f  );
			break;
		case 0xF3:
			printf( "DI" );
			DI( reg->iff1, reg->iff2 );
			break;
		case 0xF4:
			printf( "CALL P,%X", readNextWord() );
			CALLP( getNextWord(), getWordAt( reg->sp ), reg->sp, reg->pc, reg->f );
			break;
		case 0xF5:
			printf( "PUSH AF");
			PUSH( getWordAt( reg->sp ), reg->sp, reg->af );
			break;
		case 0xF6:
			printf( "OR %X", readNextByte() );
			OR( reg->a, getNextByte(), reg->f );
			break;
		case 0xF7:
			printf( "RST 30H" );
			RST( 0x30, getWordAt( reg->sp ), reg->sp, reg->pc );
			break;
		case 0xF8:
			printf( "RET M,%X***CHECK THIS", readNextWord() );
			RETM( reg->pc, getWordAt( reg->sp ), reg->sp, reg->f );
			break;
		case 0xF9:
			printf( "LD SP,HL");
			LD16( reg->sp, reg->hl );
			break;
		case 0xFA:
			printf( "JP M,%X", readNextWord() );
			JPM( reg->pc, getNextWord(), reg->f );
			break;
		case 0xFB:
			printf( "EI" );
			EI( reg->iff1, reg->iff2 );
			break;
		case 0xFC:
			printf( "CALL M,%X***Check this", readNextWord() );
			CALLM( getNextWord(), getWordAt( reg->sp ), reg->sp, reg->pc, reg->f  );
			break;
		case 0xFD:
			switch( *(getNextByte()) ){
			case 0x21:
				printf( "LD IY,+%X", readNextWord() );
				LD16( reg->iy, getNextWord() );				
				break;
			case 0x22:
				printf( "LD (%X),IY", readNextWord() );
				LD16( getWordAt( getNextWord() ) , reg->iy );	
				break;
			case 0x23:
				printf( "INC IX" );
				INC16( reg->ix );
				break;
			case 0x2A:
				printf( "LD IY(%X)", readNextWord() );
				LD16( reg->iy, getWordAt( getNextWord() ) );
				break;
			case 0x2B:
				printf( "DEC IY" );
				DEC16( reg->iy );
				break;
			case 0x34:
				printf( "INC IY+%X", readNextByte() );
				INC( getByteAt( *reg->iy + (int8_t)*( getNextByte() ) ), reg->f );
				break;
			case 0x35:
				printf( "DEC IY+%X", readNextByte() );
				DEC( getByteAt( *reg->iy + (int8_t)*( getNextByte() ) ), reg->f );
				break;
			case 0x36:
				printf( "LD (IY+%X),%02X**", (int8_t)readNextByte(), readByteAt( *reg->pc+2 ) );
				LD( getByteAt( *reg->iy + (int8_t)*( getNextByte() ) ), getByteAt( *reg->pc+2 ) );
				getNextByte();				
				break;
			case 0x46:
				printf( "LD B,(IY+%X)", (int8_t)readNextByte() );
				LD( reg->b, getByteAt( *reg->iy +(int8_t) *( getNextByte() ) ) ); 
				break;
			case 0x4E:
				printf( "LD C,(IY+%X)",(int8_t) readNextByte() );
				LD( reg->c, getByteAt( *reg->iy +(int8_t) *( getNextByte() ) ) ); 
				break;
			case 0x5E:
				printf( "LD D,(IY+%X)",(int8_t) readNextByte() );
				LD( reg->d, getByteAt( *reg->iy +(int8_t) *( getNextByte() ) ) ); 
				break;
			case 0x56:
				printf( "LD E,(IY+%X)",(int8_t) readNextByte() );
				LD( reg->e, getByteAt( *reg->iy +(int8_t) *( getNextByte() ) ) ); 
				break;
			case 0x66:
				printf( "LD H,(IY+%X)",(int8_t) readNextByte() );
				LD( reg->h, getByteAt( *reg->iy +(int8_t) *( getNextByte() ) ) ); 
				break;
			case 0x6E:
				printf( "LD L,(IY+%X)",(int8_t) readNextByte() );
				LD( reg->l, getByteAt( *reg->iy +(int8_t) *( getNextByte() ) ) ); 
				break;
			case 0x70:
				printf( "LD (IY+%X),B",(int8_t) readNextByte() );
				LD( getByteAt( *reg->iy +(int8_t) *( getNextByte() ) ), reg->b );
				break;
			case 0x71:
				printf( "LD (IY+%X),C",(int8_t) readNextByte() );
				LD( getByteAt( *reg->iy +(int8_t) *( getNextByte() ) ), reg->c );
				break;
			case 0x72:
				printf( "LD (IY+%X),D",(int8_t) readNextByte() );
				LD( getByteAt( *reg->iy +(int8_t) *( getNextByte() ) ), reg->d );
				break;
			case 0x73:
				printf( "LD (IY+%X),E",(int8_t) readNextByte() );
				LD( getByteAt( *reg->iy +(int8_t) *( getNextByte() ) ), reg->e );
				break;
			case 0x74:
				printf( "LD (IY+%X),H",(int8_t) readNextByte() );
				LD( getByteAt( *reg->iy +(int8_t) *( getNextByte() ) ), reg->h );
				break;
			case 0x75:
				printf( "LD (IY+%X),L",(int8_t) readNextByte() );
				LD( getByteAt( *reg->iy +(int8_t) *( getNextByte() ) ), reg->l );
				break;
			case 0x77:
				printf( "LD (IY+%X),A",(int8_t) readNextByte() );
				LD( getByteAt( *reg->iy +(int8_t) *( getNextByte() ) ), reg->a );
				break;	
			case 0x7E:
				printf( "LD A,(IY+%X",(int8_t) readNextByte() );
				LD( reg->a, getByteAt( *reg->iy + (int8_t)*( getNextByte() ) ) );  
				break;
			case 0x86:
				printf( "ADD (IY+%X)",(int8_t) readNextByte() );
				ADD( reg->a, getByteAt( *reg->iy +(int8_t) *( getNextByte() ) ), reg->f );
				break;
			case 0x8E:
				printf( "ADC (IY+%X)",(int8_t) readNextByte() );
				ADC( reg->a, getByteAt( *reg->iy +(int8_t) *( getNextByte() ) ), reg->f );
				break;
			case 0x96:
				printf( "SUB (IY+%X)",(int8_t) readNextByte() );
				SUB( reg->a, getByteAt( *reg->iy +(int8_t) *( getNextByte() ) ), reg->f );
				break;
			case 0xA6:
				printf( "AND A (IY+%X)",(int8_t) readNextByte() );
				AND( reg->a, getByteAt( *reg->iy +(int8_t) *( getNextByte() ) ), reg->f );
				break;
			case 0xAE:
				printf( "XOR (IY+%X)",(int8_t) readNextByte() );
				XOR( reg->a, getByteAt( *reg->iy +(int8_t) *( getNextByte() ) ) ,reg->f );
				break;
			case 0xB6:
				printf( "OR A,(IY+%X)",(int8_t) readNextByte() );
				OR( reg->a, getByteAt( *reg->iy +(int8_t) *( getNextByte() ) ), reg->f );
				break;
			case 0xBE:
				printf( "CP (IY+%X)",(int8_t) readNextByte() );
				CP( reg->a, getByteAt( *reg->iy +(int8_t) *( getNextByte() ) ) ,reg->f );
				break;
			case 0xCB:
				offset = *( getNextByte() );
				switch( *( getNextByte() ) ){
					case 0x06:
						printf( "RLC(IY+%X)", offset );
						RLC( getByteAt( *reg->iy + (int8_t) *( getNextByte() ) ), reg->f );
						break;	
					case 0x0E:
						break;
					case 0x16:
						break;
					case 0x1E:
						break;
					case 0x26:
						break;
					case 0x2E:
						break;
					case 0x3E:	
						break;
					case 0x46:
						printf( "BIT 0, (IY+%X)",(int8_t) offset );
						BIT( 0, getByteAt( *reg->iy +(int8_t) offset ), reg->f );
						break;break;	
					case 0x4E:
						printf( "BIT 1, (IY+%X)",(int8_t) offset );
						BIT( 1, getByteAt( *reg->iy +(int8_t) offset ), reg->f );
						break;
					case 0x56:
						printf( "BIT 2, (IY+%X)",(int8_t) offset );
						BIT( 2, getByteAt( *reg->iy +(int8_t) offset ), reg->f );
						break;
					case 0x5E:
					        printf( "BIT 3, (IY+%X)",(int8_t) offset );
						BIT( 3, getByteAt( *reg->iy +(int8_t) offset ), reg->f );
						break;
					case 0x66:
						printf( "BIT 4, (IY+%X)",(int8_t) offset );
						BIT( 4, getByteAt( *reg->iy +(int8_t) offset ), reg->f );
						break;
					case 0x6E:
						printf( "BIT 5, (IY+%X)",(int8_t) offset );
						BIT( 5, getByteAt( *reg->iy +(int8_t) offset ), reg->f );
						break;
					case 0x76:
						printf( "BIT 6, (IY+%X)",(int8_t) offset );
						BIT( 6, getByteAt( *reg->iy +(int8_t) offset ), reg->f );
						break;
					case 0x7E:
						printf( "BIT 7, (IY+%X)",(int8_t) offset );
						BIT( 7, getByteAt( *reg->iy +(int8_t) offset ), reg->f );
						break;
					case 0x86:
						printf( "RES 0, (IY+%X)",(int8_t) offset );
						RES( 0, getByteAt( *reg->iy +(int8_t) offset ) );
						break;	
					case 0x8E:
						printf( "RES 1, (IY+%X)",(int8_t) offset );
						RES( 1, getByteAt( *reg->iy +(int8_t) offset ) );
						break;
					case 0x96:
						printf( "RES 2, (IY+%X)",(int8_t) offset );
						RES( 2, getByteAt( *reg->iy +(int8_t) offset ) );
						break;
					case 0x9E:
					        printf( "RES 3, (IY+%X)",(int8_t) offset );
						RES( 3, getByteAt( *reg->iy +(int8_t) offset ) );
						break;
					case 0xA6:
						printf( "RES 4, (IY+%d)",(int8_t) offset );
						RES( 4, getByteAt( *reg->iy +(int8_t) offset ) );
						break;
					case 0xAE:
						printf( "RES 5, (IY+%X)",(int8_t) offset );
						RES( 5, getByteAt( *reg->iy +(int8_t) offset ) );
						break;
					case 0xB6:
						printf( "RES 6, (IY+%X)",(int8_t) offset );
						RES( 6, getByteAt( *reg->iy +(int8_t) offset ) );
						break;
					case 0xBE:
						printf( "RES 7, (IY+%X)",(int8_t) offset );
						RES( 7, getByteAt( *reg->iy +(int8_t) offset ) );
						break;
					case 0xC6:
						printf( "SET 0, (IY+%X)", (int8_t)offset );
						SET( 0, getByteAt( *reg->iy +(int8_t) offset ) );
						break;	
					case 0xCE:
						printf( "SET 1, (IY+%X) ",(int8_t) offset );
						SET( 1, getByteAt( *reg->iy + (int8_t)offset ) );
						break;
					case 0xD6:
						printf( "SET 2, (IY+%X) ",(int8_t) offset );	
						SET( 2, getByteAt( *reg->iy +(int8_t) offset ) );
						break;
					case 0xDE:
					        printf( "SET 3, (IY+%X) ",(int8_t) offset );
						SET( 3, getByteAt( *reg->iy +(int8_t) offset ) );
						break;
					case 0xE6:
						printf( "SET 4, (IY+%X) ",(int8_t) offset );
						SET( 4, getByteAt( *reg->iy +(int8_t) offset ) );
						break;
					case 0xEE:
						printf( "SET 5, (IY+%X) ",(int8_t) offset );
						SET( 5, getByteAt( *reg->iy +(int8_t) offset ) );
						break;
					case 0xF6:
						printf( "SET 6, (IY+%X) ",(int8_t) offset );
						SET( 6, getByteAt( *reg->iy +(int8_t) offset ) );
						break;
					case 0xFE:
						printf( "SET 7, (IY+%X) ",(int8_t) offset );
						SET( 7, getByteAt( *reg->iy +(int8_t) offset ) );
						break;
					default:
						break;
				}
				offset = 0x00;
				break;
			case 0xE1:
				printf( "POP IY");
				POP( getWordAt( reg->sp ), reg->sp, reg->iy );
				break;
			case 0xE3:
				printf( "EX (SP),IY");
				EX( getWordAt( reg->sp ), reg->iy );
				break;
			case 0xE5:
				printf( "PUSH IY" );
				PUSH( getWordAt( reg->sp ), reg->sp, reg->iy );
				break;
			case 0xE9:
				printf( "JP(IY)" );
				JP( reg->pc, getWordAt( reg->iy ) );
				break;
			case 0xF9:
				printf( "LD SP,IY");
				LD16( reg->sp, reg->iy );
				break;
			default:
				break;
			}
			break;
		case 0xFE:
			printf( "CP %X", readNextByte() );
			CP( reg->a, getNextByte(), reg->f );
			break;
		case 0xFF:
			printf( "RST 38H");
			RST( 0x38, getWordAt( reg->sp ), reg->sp, reg->pc );
			break;
		default:
			break;
		} 
		
		printf( "\n");
}

// Return the 16-bit value of two 8-bit combined registers
uint16_t byteToWord( uint8_t *byte1, uint8_t *byte2 ){
	uint16_t word = 0x00;
	
	word = (word | *byte1);

	word <<= 8;

	word = (word | *byte2);

	return word;
}

// Returns the LO byte of a word *could make into macro functions *
uint8_t getLOByte( uint16_t *word ){
	return (*word ^ 0xff00); // XOR the word with 1111111100000000 to zeros the HO byte
}

// Returns the HO byte of a word
uint8_t getHObyte( uint16_t *word ){
	return ((*word ^ 0xff) >> 8); // XOR the word with 1111111100000000 to zeros the HO byte then bit shift 8 places to the right
}

// Memory handling

// Read byte from memory given 16-bit address
uint8_t* getNextByte(){
	// Dereference into the memory location and return the value, progress pc by 1
	return getByte( ++*reg->pc );
}

// Read the next byte along, does not progress pc
uint8_t readNextByte(){
	// Dereference into the memory location and return the value, progress pc by 1
	return *( getByte( *reg->pc+1 ) );
}

uint16_t* getNextWord(){
	// Dereference into the memory location and return the word of the next two bytes	
	uint16_t* word = getWord( ++*reg->pc );
	// Increment pc again to point at at the second byte in the word, ready for next increment by execution loop
	++*reg->pc;
	return word;
}

// read next word along, does not progress pc
uint16_t readNextWord(){
	// Dereference into the memory location and return the word of the next to bytes
	return *( getWord( *reg->pc+1 ) );
}

uint8_t* getByteAt( uint16_t addrs ){

	uint8_t* byte = getByte( addrs );	
	// Return a pointer to the byte at a given 16-bit memory address	
	return byte;	
}

uint16_t* getWordAt( uint16_t* addrs ){
	return getWord( *addrs );
}
// Read word at memory location
uint16_t readWordAt( uint16_t *addrs ){
	return *( getWord( *addrs ) );
}
uint16_t readByteAt( uint16_t addrs ){
	return *( getByte( addrs ) );
}
// Write byte to memory given 16-bit address
void writeByte( uint16_t addrs, uint8_t val ){
	//setByte(addrs, val);
}


