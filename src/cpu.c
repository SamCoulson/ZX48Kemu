#include "../include/cpu.h"
#include <assert.h>
#include <stdio.h>
#include <conio.h>
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

#include "../include/ULA.h"

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
	// 8-bit Special registers - interrupt vector
	.i = &ir._ir[1], .r = &ir._ir[0], .ir = &ir.ir,
	// 16-bit index registers
	.ix = &ix, .iy = &iy,
      	// 16-bit stack pointer and program pointer
	.sp = &sp, .pc = &pc,
	// IFF (Interrupt enabled flip-flop)
	.iff1 = &iff1, .iff2 = &iff2 	
};

// Pointer to the register structure
Registers* reg = &registers;

// IO ports *** The CPU has 256 addressable ports *** 
// This might be excessive for emualtion but all IO will park data at these location for read/write by external modules
uint8_t ports[256] = {0};
static uint8_t* IOport = ports; 

// Static structure for registers
int initCPU(){
	
	return 0;
}
//  Set the PC to point to a location in memory
void run( uint16_t addrs ){
	
	freopen( "CON" ,"w", stdout);
	// Begin executing from 0x0000
	*reg->pc = addrs;

	// DEBUG
	*reg->pc = 0x0000;

	//While less that 16k rom
	while( *reg->pc < 0xFFFF ){			

		getchar();
		//if( *reg->pc == 0x0EE7 )
		//	break;
		// Execute the instruction pointed to by pc
		execute( &totalMem[*reg->pc]  );

		// Execute the opcode pointed to by program counter
		getNextByte();
	
		// Do interrupts
		// read( &IOport[0xfe] );
		// readVideoRAM( totalMem );
printf("\nHL = %X\n", *reg->hl )md

printf( "\n\nStack pointer points to %X and contains %X\n", *reg->sp, readWordAt( reg->sp ) );
		// Could use: IF address is RAM-TEST then make PC skip to avoid the unnecassry check
		if( *reg->pc == 0x11CB ){
			*reg->pc = 0x11EF;
			// Set HL to +FFFF fro RAM-TOP
			*reg->hl = 0xFFFF;	
		}

		//system("cls");
		/*
		printf("\nMain registers\tAlternate registers\n");
		printf("AF: %02x %02x", regA, regF );  
		printf("\tAF': %02x %02x\n",aregA, aregF );
		printf("BC: %02x %02x",regB, regC );  
		printf("\tBC': %02x %02x\n",aregB, aregC );
		printf("DE: %02x %02x",regD, regE );  
		printf("\tDE': %02x %02x\n", aregD, aregE );

		printf("HL: %02x %02x",regH, regL );  
		printf("\tHL': %02x %02x\n", aregH, aregL );

		printf("\nSP: %04x",sp );
	      	printf( "\nPC: %04x",pc );	

		printf("\nIX: %04x",indexIX);
		printf("\tIY: %04x", indexIY);

		printf("\nI: %02x", regI);
	        printf("\t\tR: %02x\n", regR);
	
		printf("\nEFF 1: %d", IFF2);
		printf("\tEFF 2: %d\n", IFF1);
		*/
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
			LD( getByteAt( reg->bc ) ,reg->a );
			break;
		case 0x03:
			printf( "INC BC" );
			INC16( reg->bc );
			break;
		case 0x04:
			printf( "INC B" );
			INC( reg->b );
			break;
		case 0x05:
			printf( "DEC B" );
			DEC( reg->b );
			break;
		case 0x06:
			printf( "LD B,+%X", readNextByte() );
			LD( reg->b, getNextByte() );
			break;/*
		case 0x07:
			break;*/
		case 0x08:
			printf( "EX AF,AF" );
			EX( reg->af, reg->altaf );
			break;
		case 0x09:
			printf("ADD HL,BC");
			ADD16( reg->hl, reg->bc, reg->f );
			break;
		case 0x0A:
			printf( "LD A,(BC)" );
			LD( reg->a, getByteAt( reg->bc ) );
			break;
		case 0x0B:
			printf("DEC BC");
			DEC16( reg->bc );
			break;
		case 0x0C:
			printf( "INC C" );
			INC( reg->c );
			break;
		case 0x0D:
			printf( "DEC C" );
			DEC( reg->c );
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
			LD( getByteAt( reg->bc ) ,reg->a );
			break;
		case 0x13:
			printf( "INC DE" );
			INC16( reg->de );
			break;
		case 0x14:
			printf( "INC D" );
			INC( reg->d );
			break;
		case 0x15:
			printf( "DEC D" );
			DEC( reg->d );
			break;
		case 0x16:
			printf( "LD D,+%X", readNextByte() );
			LD( reg->d, getNextByte() );
			break;/*
		case 0x17:
			break;*/
		case 0x18:
			printf( "JR %d", readNextByte() );
			JR( getNextByte(), reg->pc );
			break;
		case 0x19:
			printf("ADD HL,DE");
			ADD16( reg->hl, reg->de, reg->f );
			break;
		case 0x1A:
			printf( "LD A,(DE)" );
			LD( reg->a, getByteAt( reg->de ) );
			break;
		case 0x1B:
			printf( "DEC DE" );
			DEC16( reg->de );
			break;
		case 0x1C:
			printf( "INC E" );
			INC( reg->e );
			break;
		case 0x1D:
			printf( "DEC E" );
			DEC( reg->e );
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
			INC( reg->h );
			break;
		case 0x25:
			printf( "DEC H" );
			DEC( reg->h );
			break;
		case 0x26:
			printf( "LD H,+%X", readNextByte() );
			LD( reg->h, getNextByte() );
			break;/*
		case 0x27:
			break;*/
		case 0x28:
			printf( "JR Z,%d", (int8_t)readNextByte() );
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
			INC( reg->l );
			break;
		case 0x2D:
			printf( "DEC L" );
			DEC( reg->l );
			break;
		case 0x2E:
			printf( "LD L+,%X", readNextByte() );
			LD( reg->l, getNextByte() );
			break;/*
		case 0x2F:
			break;*/
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
			LD( getByteAt( getNextWord() ) ,reg->a );
			break;
		case 0x33:
			printf( "INC SP" );
			INC16( reg->sp );
			break;
		case 0x34:
			printf( "INC (HL)" );
			INC( getByteAt( reg->hl ) );
			break;
		case 0x35:
			printf("DEC (HL)" );
			DEC( getByteAt( reg->hl ) );
			break;
		case 0x36:
			printf( "LD (HL),+%X", readNextByte() );
			LD( getByteAt( reg->hl ), getNextByte() );
			break;
		case 0x37:
			//printf( "SCF";
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
			LD( reg->a, getByteAt( getNextWord() ) );
			break;
		case 0x3B:
			printf("DEC SP");
			DEC16( reg->sp );
			break;
		case 0x3C:
			printf( "INC A" );
			INC( reg->a );
			break;
		case 0x3D:
			printf( "DEC A" );
			DEC( reg->a );
			break;
		case 0x3E:
			printf( "LD A,+%X", readNextByte() );
			LD( reg->a, getNextByte() );
			break;/*
		case 0x3F:
			//printf( "CCF";
		break;*/
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
			LD( reg->b, getByteAt( reg->hl ) );
			break;
		case 0x47:
			printf( "LD B,A");
			LD( reg->b, reg->a );
			break;
		case 0x48:
			printf( "LD C,B");
			LD( reg->b, reg->b );
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
			printf( "LD C,(HL)");
			LD( reg->c, getByteAt( reg->hl ) );
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
			printf( "LD D,(%02X)", readByteAt( reg->hl ));
			LD( reg->b, getByteAt( reg->hl ) );
			break;
		case 0x57:
			printf( "LD E,A" );
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
			LD(reg->e, reg->d );
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
			printf( "LD E,(%02X)", readByteAt( reg->hl ) );
			LD( reg->e, getByteAt( reg->hl ) );
			break;
		case 0x5F:
			printf( "LD D,A" );
			LD( reg->d, reg->a );
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
			printf( "LD H,((%02X)", readByteAt( reg->hl ) );
			LD( reg->h, getByteAt( reg->hl ) );
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
			printf( "LD L,(%X)", readByteAt( reg->hl ) );
			LD( reg->l, getByteAt( reg->hl ) );
			break;
		case 0x6F:
			printf( "LD L,A" );
			LD( reg->l, reg->a );
			break;
		case 0x70:
			printf( "(HL),B");
			LD( getByteAt( reg->hl ) ,reg->b );
			break;
		case 0x71:
			printf( "(HL),C");
			LD( getByteAt( reg->hl ), reg->c );
			break;
		case 0x72:
			printf( "(HL),D");
			LD( getByteAt( reg->hl ), reg->d );
			break;
		case 0x73:
			printf( "(HL),E");
			LD( getByteAt( reg->hl ), reg->e );
			break;
		case 0x74:
			printf( "(HL),H");
			LD( getByteAt( reg->hl ), reg->h );
			break;
		case 0x75:
			printf( "(HL),L");
			LD( getByteAt( reg->hl ), reg->l );
			break;/*
		case 0x76:
			cout << "HALT");
		break;*/
		case 0x77:
			printf( "LD (HL),A");
			LD( getByteAt( reg->hl ) ,reg->a );
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
			printf( "LD A,(%02X)", readByteAt( reg->hl ) );
			LD( reg->a, getByteAt( reg->hl ) );
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
			ADD( reg->a, getByteAt( reg->hl ), reg->f );
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
			ADC( reg->a, getByteAt( reg->hl ), reg->f );
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
			printf( "SUB (%X)", readNextByte() );
			SUB( reg->a, getNextByte(), reg->f );
			break;
		case 0x97:
			printf( "SUB A" );
			SUB( reg->a, reg->a, reg->f );
			break;/*
		case 0x98:
			break;
		case 0x99:
			break;
		case 0x9A:
			break;
		case 0x9B:
			break;
		case 0x9C:
			break;
		case 0x9D:
			break;
		case 0x9E:
			break;
		case 0x9F:
			break;*/
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
			AND( reg->a, getByteAt( reg->hl ), reg->f );
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
			XOR( reg->a, getByteAt( reg->hl ), reg->f );
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
			OR( reg->a, getByteAt( reg->hl ), reg->f );
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
			CP( reg->a, getByteAt( reg->hl ), reg->f );
			break;
		case 0xBF:
			printf( "CP A" );
			CP( reg->a, reg->a, reg->f );
			break;/*
		case 0xC0:
			//printf( "RET NZ";
		break;*/
		case 0xC1:
			printf( "POP BC");
			POP( getWordAt( reg->sp ) ,reg->sp, reg->bc );
			break;
		case 0xC2:
			//printf( "JP NZ %d%d",(int)readByte( pc+2 ), (int)readByte( pc+1 ) );
			//JPCondition( "NZ", readByte( ++pc ), readByte( ++pc ) );
			break;
		case 0xC3:
			printf( "JP %X", readNextWord() );
			JP( reg->pc, getNextWord() );
			break;/*
		case 0xC4:
			break;*/
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
			RST( 0x00, getWordAt( reg->sp ), reg->pc );
			break;
		case 0xC8:
			printf( "RET Z" );
			RETZ( reg->pc, getWordAt( reg->sp ), reg->f );
			break;
		case 0xC9:
			printf( "RET");
			RET( reg->pc, getWordAt( reg->sp ), reg->sp );
			break;/*
		case 0xCA:
			break;*/
		case 0xCB:
			switch( *( getNextByte() ) ){
				case 0x06:
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
					printf( "BIT 0" );
					BIT( 0, getByteAt( reg->hl ), reg->f );
					break;	
				case 0x4E:
					printf( "BIT 1" );
					BIT( 1, getByteAt( reg->hl ), reg->f );
					break;
				case 0x56:
					printf( "BIT 2" );
					BIT( 2, getByteAt( reg->hl ), reg->f );
					break;
				case 0x5E:
					printf( "BIT 3" );
					BIT( 3, getByteAt( reg->hl ), reg->f );
					break;
				case 0x66:
					printf( "BIT 4" );
					BIT( 4, getByteAt( reg->hl ), reg->f );
					break;
				case 0x6E:
					printf( "BIT 5" );
					BIT( 5, getByteAt( reg->hl ), reg->f );
					break;
				case 0x76:
					printf( "BIT 6" );
					BIT( 6, getByteAt( reg->hl ), reg->f );
					break;
				case 0x7E:
					printf( "BIT 7" );
					BIT( 7, getByteAt( reg->hl ), reg->f );
					break;
				case 0x86:
					printf( "RES 0" );
					RES( 0, getByteAt( reg->hl ) );
					break;	
				case 0x8E:
					printf( "RES 1" );
					RES( 1, getByteAt( reg->hl ) );
					break;
				case 0x96:
					printf( "RES 2" );
					RES( 2, getByteAt( reg->hl ) );
					break;
				case 0x9E:
					printf( "RES 3" );
					RES( 3, getByteAt( reg->hl ) );
					break;
				case 0xA6:
					printf( "RES 4" );
					RES( 4, getByteAt( reg->hl ) );
					break;
				case 0xAE:
					printf( "RES 5" );
					RES( 5, getByteAt( reg->hl ) );
					break;
				case 0xB6:
					printf( "RES 6" );
					RES( 6, getByteAt( reg->hl ) );
					break;
				case 0xBE:
					printf( "RES 7" );
					RES( 7, getByteAt( reg->hl ) );
					break;
				case 0xC6:
					printf( "SET 0" );
					SET( 0, getByteAt( reg->hl ) );
					break;	
				case 0xCE:
					printf( "SET 1" );
					SET( 1, getByteAt( reg->hl ) );
					break;
				case 0xD6:
					printf( "SET 2" );	
					SET( 2, getByteAt( reg->hl ) );
					break;
				case 0xDE:
					printf( "SET 3" );
					SET( 3, getByteAt( reg->hl ) );
					break;
				case 0xE6:
					printf( "SET 4" );
					SET( 4, getByteAt( reg->hl ) );
					break;
				case 0xEE:
					printf( "SET 5" );
					SET( 5, getByteAt( reg->hl ) );
					break;
				case 0xF6:
					printf( "SET 6" );
					SET( 6, getByteAt( reg->hl ) );
					break;
				case 0xFE:
					printf( "SET 7" );
					SET( 7, getByteAt( reg->hl ) );
					break;
				default:
					break;
			}
			break;/*
		case 0xCC:
			break;*/
		case 0xCD:
			printf( "CALL %X", readNextWord() );
			CALL( getNextWord(), getWordAt( reg->sp ), reg->sp, reg->pc );
			break;
		case 0xCE:
			printf( "ADC A,%d", readNextByte() );
			ADC( reg->a, getNextByte(), reg->f );
			break;
		case 0xCF:
			printf( "RST 08H" );
			RST( 0x08, getWordAt( reg->sp ), reg->pc );
			break;
		case 0xD0:
			printf( "RET NC" );
			RETNC( reg->pc, getWordAt( reg->sp ), reg->f );
			break;
		case 0xD1:
			printf( "POP DE");
			POP( getWordAt( reg->sp ) ,reg->sp, reg->de );
			break;/*
		case 0xD2:
			break;*/
		case 0xD3:
			printf("OUT (+%X),A", readNextByte() );
			OUTA( &IOport[ *( getNextByte() ) ], reg->a );
			break;/*
		case 0xD4:
			//printf( "CALL NC,%X%X");
			break;*/
		case 0xD5:
			printf( "PUSH DE");
			PUSH( getWordAt( reg->sp ), reg->sp, reg->de );
			break;/*
		case 0xD6:
			break;*/
		case 0xD7:
			printf( "RST 10" );
			RST( 0x10, reg->sp, reg->pc );
			break;/*
		case 0xD8:
			//printf( "RET C");
			break;*/
		case 0xD9:
			printf( "EXX" );
			EXX( reg->bc, reg->de, reg->hl, reg->altbc, reg->altde, reg->althl );
			break;/*
		case 0xDA:
			break;
		case 0xDB:
			break;
		case 0xDC:
			break;*/
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
				printf( "INC IX+%X", readNextByte() );
				INC( getByteAt( reg->ix + *( getNextByte() ) ) );
				break;
			case 0x35:
				printf( "DEC IX +%d", readNextByte() );
				DEC( getByteAt( reg->ix + *( getNextByte() ) ) );
				break;
			case 0x36:
				printf( "LD (IX+%d)", readNextByte() );
				LD( getByteAt( reg->ix + *( getNextByte() ) ), getNextByte() );
				break;
			case 0x46:
				printf( "LD B,(IX+%d)", readNextByte() );
				LD( reg->b, getByteAt( reg->ix + *(getNextByte()) ) ); 
				break;
			case 0x4E:
				printf( "LD C,(IX+%d)", readNextByte() );
				LD( reg->c, getByteAt( reg->ix + *(getNextByte() ) ) ); 
				break;
			case 0x5E:
				printf( "LD D,(IX+%d)", readNextByte() );
				LD( reg->d, getByteAt( reg->ix + *(getNextByte() ) ) ); 
				break;
			case 0x56:
				printf( "LD E,(IX+%d)", readNextByte() );
				LD( reg->e, getByteAt( reg->ix + *(getNextByte() ) ) ); 
				break;
			case 0x66:
				printf( "LD H,(IX+%d)", readNextByte() );
				LD( reg->h, getByteAt( reg->ix + *(getNextByte() ) ) ); 
				break;
			case 0x6E:
				printf( "LD L,(IX+%d)", readNextByte() );
				LD( reg->l, getByteAt( reg->ix + *( getNextByte() ) ) ); 
				break;
			case 0x70:
				printf( "LD (IX+%d),B", readNextByte() );
				LD( getByteAt( reg->ix + *( getNextByte() ) ), reg->b );
				break;
			case 0x71:
				printf( "LD (IX+%d),C", readNextByte() );
				LD( getByteAt( reg->ix + *( getNextByte() ) ), reg->c );				
				break;
			case 0x72:
				printf( "LD (IX+%d),D", readNextByte() );
				LD( getByteAt( reg->ix + *( getNextByte() ) ), reg->d );
				break;
			case 0x73:
				printf( "LD (IX+%d),E", readNextByte() );
				LD( getByteAt( reg->ix + *( getNextByte() ) ), reg->e );
				break;
			case 0x74:
				printf( "LD (IX+%d),H", readNextByte() );
				LD( getByteAt( reg->ix + *( getNextByte() ) ), reg->h );
				break;
			case 0x75:
				printf( "LD (IX+%d),L", readNextByte() );
				LD( getByteAt( reg->ix + *( getNextByte() ) ), reg->l );
				break;
			case 0x77:
				printf( "LD (IX+%d),A", readNextByte() );
				LD( getByteAt( reg->ix + *( getNextByte() ) ), reg->a );
				break;
			case 0x7E:
				printf( "LD A,(IX+%d)", readNextByte() );
				LD( reg->a, getByteAt( reg->ix + *( getNextByte() ) ) ); 
				break;
			case 0x86:
				printf( "ADD (IX+%d)", readNextByte() );
				ADD( reg->a, getByteAt( reg->ix + *( getNextByte() ) ), reg->f );
				break;
			case 0x8E:
				printf( "ADC (IX+%d)", readNextByte() );
				ADC( reg->a, getByteAt( reg->ix + *( getNextByte() ) ), reg->f );
				break;
			case 0x96:
				printf( "SUB (IX+%X)", readNextByte() );
				SUB( reg->a, getByteAt( reg->ix + *( getNextByte() ) ), reg->f );
				break;
			case 0xA6:
				printf( "AND A (IX+%X)",readNextByte() );
				AND( reg->a, getByteAt( reg->ix + *( getNextByte() ) ), reg->f );
				break;
			case 0xAE:
				printf( "XOR (IX+%X)", readNextByte() );
				XOR( reg->a, getByteAt( reg->ix + *( getNextByte() ) ), reg->f );
				break;
			case 0xB6:
				printf( "OR A,(IX+%d)", readNextByte() );
				OR( reg->a, getByteAt( reg->ix + *( getNextByte() ) ), reg->f );
				break;
			case 0xBE:
				printf( "CP (IX+%X)", readNextByte() );
				CP( reg->a, getByteAt( reg->ix + *( getNextByte() ) ) ,reg->f );
				break;
			case 0xCB:
				offset = *( getNextByte() );
				switch( *( getNextByte() ) ){
					case 0x06:
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
						printf( "BIT 0" );
						BIT( 0, getByteAt( reg->ix + offset ), reg->f );
						break;	
					case 0x4E:
						printf( "BIT 1" );
						BIT( 1, getByteAt( reg->ix + offset ), reg->f );
						break;
					case 0x56:
						printf( "BIT 2" );
						BIT( 2, getByteAt( reg->ix + offset ), reg->f );
						break;
					case 0x5E:
					        printf( "BIT 3" );
						BIT( 3, getByteAt( reg->ix + offset ), reg->f );
						break;
					case 0x66:
						printf( "BIT 4" );
						BIT( 4, getByteAt( reg->ix + offset ), reg->f );
						break;
					case 0x6E:
						printf( "BIT 5" );
						BIT( 5, getByteAt( reg->ix + offset ), reg->f );
						break;
					case 0x76:
						printf( "BIT 6" );
						BIT( 6, getByteAt( reg->ix + offset ), reg->f );
						break;
					case 0x7E:
						printf( "BIT 7" );
						BIT( 7, getByteAt( reg->ix + offset ), reg->f );
						break;
					case 0x86:
						printf( "RES 0" );
						RES( 0, getByteAt( reg->ix + offset ) );
						break;	
					case 0x8E:
						printf( "RES 1" );
						RES( 1, getByteAt( reg->ix + offset ) );
						break;
					case 0x96:
						printf( "RES 2" );
						RES( 2, getByteAt( reg->ix + offset ) );
						break;
					case 0x9E:
					        printf( "RES 3" );
						RES( 3, getByteAt( reg->ix + offset ) );
						break;
					case 0xA6:
						printf( "RES 4" );
						RES( 4, getByteAt( reg->ix + offset ) );
						break;
					case 0xAE:
						printf( "RES 5" );
						RES( 5, getByteAt( reg->ix + offset ) );
						break;
					case 0xB6:
						printf( "RES 6" );
						RES( 6, getByteAt( reg->ix + offset ) );
						break;
					case 0xBE:
						printf( "RES 7" );
						RES( 7, getByteAt( reg->ix + offset ) );
						break;
					case 0xC6:
						printf( "SET 0" );
						SET( 0, getByteAt( reg->ix + offset ) );
						break;	
					case 0xCE:
						printf( "SET 1" );
						SET( 1, getByteAt( reg->ix + offset ) );
						break;
					case 0xD6:
						printf( "SET 2" );	
						SET( 2, getByteAt( reg->ix + offset ) );
						break;
					case 0xDE:
					        printf( "SET 3" );
						SET( 3, getByteAt( reg->ix + offset ) );
						break;
					case 0xE6:
						printf( "SET 4" );
						SET( 4, getByteAt( reg->ix + offset ) );
						break;
					case 0xEE:
						printf( "SET 5" );
						SET( 5, getByteAt( reg->ix + offset ) );
						break;
					case 0xF6:
						printf( "SET 6" );
						SET( 6, getByteAt( reg->ix + offset ) );
						break;
					case 0xFE:
						printf( "SET 7" );
						SET( 7, getByteAt( reg->ix + offset ) );
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
			case 0xF9:
				printf( "LD SP,IX" );
				LD16( reg->sp, reg->ix );
				break;
			default:
				break;
			}
			break;/*
		case 0xDE:
			break;*/
		case 0xDF:
			printf( "RST 18H" );
			RST( 0x18, reg->sp, reg->pc );
			break;/*
		case 0xE0:
			break;*/
		case 0xE1:
			printf( "POP HL");
			POP( getWordAt( reg->sp ) ,reg->sp, reg->hl );
			break;/*
		case 0xE2:
			break;*/
		case 0xE3:
			//printf( "EX (SP),HL");
			//EXHLToAddrsOfSP();
			break;/*
		case 0xE4:
			break;*/
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
			RST( 0x20, reg->sp, reg->pc );
			break;/*
		case 0xE8:
			//printf("");
			break;*/
		case 0xE9:
			printf( "JP (HL)");
			printf( "\n%X %X\n", *reg->hl, readWordAt( reg->hl ) );
			JP( reg->pc, getWordAt( reg->hl ) );
			break;/*
		case 0xEA:
			//printf( "");
			break;*/
		case 0xEB:
			printf( "EX DE,HL");
			EX( reg->de, reg->hl );
			break;/*
		case 0xEC:
			//printf("");
			break;*/
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
			case 0x47:
				printf( "LD I,A");
				LD( reg->i, reg->a );
				break;
			case 0x4B:
				printf( "LD BC(%X)", readNextWord() );
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
				printf( "IM1" ); 
				IM1( reg->iff1 );
				break;
			case 0x57:
				printf( "LD A,I");
				LDAIR( reg->a, reg->i, reg->f, reg->iff2 );
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
				LDIR( getWordAt( reg->hl ), getWordAt( reg->de ), reg->hl, reg->de, reg->bc, reg->f );
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
			RST( 0x28, reg->sp, reg->pc );
			break;/*
		case 0xF0:
			break;*/
		case 0xF1:
			printf( "POP AF");
			POP( getWordAt( reg->sp ) ,reg->sp, reg->af );
			break;/*
		case 0xF2:
			break;*/
		case 0xF3:
			printf( "DI" );
			DI( reg->iff1, reg->iff2 );
		break;/*
		case 0xF4:
			break;*/
		case 0xF5:
			printf( "PUSH AF");
			PUSH( getWordAt( reg->sp ), reg->sp, reg->af );
			break;
		case 0xF6:
			printf( "OR %d", readNextByte() );
			OR( reg->a, getNextByte(), reg->f );
			break;
		case 0xF7:
			printf( "RST 30H" );
			RST( 0x30, reg->sp, reg->pc );
			break;/*
		case 0xF8:
			break;*/
		case 0xF9:
			printf( "LD SP,HL");
			LD16( reg->sp, reg->hl );
			break;/*
		case 0xFA:
			break;*/
		case 0xFB:
			printf( "EI" );
			//EI( reg->iff2 );
			break;/*
		case 0xFC:
			break;*/
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
				printf( "INC IY+%d", readNextByte() );
				INC( getByteAt( reg->iy + *( getNextByte() ) ) );
				break;
			case 0x35:
				printf( "DEC IY+%d", readNextByte() );
				DEC( getByteAt( reg->iy + *( getNextByte() ) ) );
				break;
			case 0x36:
				printf( "LD (IX+%d)", readNextByte() );
				LD( getByteAt( reg->iy + *( getNextByte() ) ), getNextByte() );				
				break;
			case 0x46:
				printf( "LD B,(IY+%d", readNextByte() );
				LD( reg->b, getByteAt( reg->iy + *( getNextByte() ) ) ); 
				break;
			case 0x4E:
				printf( "LD C,(IY+%d", readNextByte() );
				LD( reg->c, getByteAt( reg->iy + *( getNextByte() ) ) ); 
				break;
			case 0x5E:
				printf( "LD D,(IY+%d", readNextByte() );
				LD( reg->d, getByteAt( reg->iy + *( getNextByte() ) ) ); 
				break;
			case 0x56:
				printf( "LD E,(IY+%d", readNextByte() );
				LD( reg->e, getByteAt( reg->iy + *( getNextByte() ) ) ); 
				break;
			case 0x66:
				printf( "LD H,(IY+%d", readNextByte() );
				LD( reg->h, getByteAt( reg->iy + *( getNextByte() ) ) ); 
				break;
			case 0x6E:
				printf( "LD L,(IY+%d", readNextByte() );
				LD( reg->l, getByteAt( reg->iy + *( getNextByte() ) ) ); 
				break;
			case 0x70:
				printf( "LD (IY+%d),B", readNextByte() );
				LD( getByteAt( reg->iy + *( getNextByte() ) ), reg->b );
				break;
			case 0x71:
				printf( "LD (IY+%d),C", readNextByte() );
				LD( getByteAt( reg->iy + *( getNextByte() ) ), reg->c );
				break;
			case 0x72:
				printf( "LD (IY+%d),D", readNextByte() );
				LD( getByteAt( reg->iy + *( getNextByte() ) ), reg->d );
				break;
			case 0x73:
				printf( "LD (IY+%d),E", readNextByte() );
				LD( getByteAt( reg->iy + *( getNextByte() ) ), reg->e );
				break;
			case 0x74:
				printf( "LD (IY+%d),H", readNextByte() );
				LD( getByteAt( reg->iy + *( getNextByte() ) ), reg->h );
				break;
			case 0x75:
				printf( "LD (IY+%d),L", readNextByte() );
				LD( getByteAt( reg->iy + *( getNextByte() ) ), reg->l );
				break;
			case 0x77:
				printf( "LD (IY+%d),A", readNextByte() );
				LD( getByteAt( reg->iy + *( getNextByte() ) ), reg->a );
				break;	
			case 0x7E:
				printf( "LD A,(IY+%d", readNextByte() );
				LD( reg->a, getByteAt( reg->iy + *( getNextByte() ) ) );  
				break;
			case 0x86:
				printf( "ADD (IY+%d)", readNextByte() );
				ADD( reg->a, getByteAt( reg->iy + *( getNextByte() ) ), reg->f );
				break;
			case 0x8E:
				printf( "ADC (IY+%d)", readNextByte() );
				ADC( reg->a, getByteAt( reg->iy + *( getNextByte() ) ), reg->f );
				break;
			case 0x96:
				printf( "SUB (IY+%X)", readNextByte() );
				SUB( reg->a, getByteAt( reg->iy + *( getNextByte() ) ), reg->f );
				break;
			case 0xA6:
				printf( "AND A (IY+%X)",readNextByte() );
				AND( reg->a, getByteAt( reg->iy + *( getNextByte() ) ), reg->f );
				break;
			case 0xAE:
				printf( "XOR (IY+%X)", readNextByte() );
				XOR( reg->a, getByteAt( reg->iy + *( getNextByte() ) ) ,reg->f );
				break;
			case 0xB6:
				printf( "OR A,(IY+%d)", readNextByte() );
				OR( reg->a, getByteAt( reg->iy + *( getNextByte() ) ), reg->f );
				break;
			case 0xBE:
				printf( "CP (IY+%X)", readNextByte() );
				CP( reg->a, getByteAt( reg->iy + *( getNextByte() ) ) ,reg->f );
				break;
			case 0xCB:
				offset = *( getNextByte() );
				switch( *( getNextByte() ) ){
					case 0x06:
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
						printf( "BIT 0" );
						BIT( 0, getByteAt( reg->iy + offset ), reg->f );
						break;break;	
					case 0x4E:
						printf( "BIT 1" );
						BIT( 1, getByteAt( reg->iy + offset ), reg->f );
						break;
					case 0x56:
						printf( "BIT 2" );
						BIT( 2, getByteAt( reg->iy + offset ), reg->f );
						break;
					case 0x5E:
					        printf( "BIT 3" );
						BIT( 3, getByteAt( reg->iy + offset ), reg->f );
						break;
					case 0x66:
						printf( "BIT 4" );
						BIT( 4, getByteAt( reg->iy + offset ), reg->f );
						break;
					case 0x6E:
						printf( "BIT 5" );
						BIT( 5, getByteAt( reg->iy + offset ), reg->f );
						break;
					case 0x76:
						printf( "BIT 6" );
						BIT( 6, getByteAt( reg->iy + offset ), reg->f );
						break;
					case 0x7E:
						printf( "BIT 7" );
						BIT( 7, getByteAt( reg->iy + offset ), reg->f );
						break;
					case 0x86:
						printf( "RES 0" );
						RES( 0, getByteAt( reg->iy + offset ) );
						break;	
					case 0x8E:
						printf( "RES 1" );
						RES( 1, getByteAt( reg->iy + offset ) );
						break;
					case 0x96:
						printf( "RES 2" );
						RES( 2, getByteAt( reg->iy + offset ) );
						break;
					case 0x9E:
					        printf( "RES 3" );
						RES( 3, getByteAt( reg->iy + offset ) );
						break;
					case 0xA6:
						printf( "RES 4" );
						RES( 4, getByteAt( reg->iy + offset ) );
						break;
					case 0xAE:
						printf( "RES 5" );
						RES( 5, getByteAt( reg->iy + offset ) );
						break;
					case 0xB6:
						printf( "RES 6" );
						RES( 6, getByteAt( reg->iy + offset ) );
						break;
					case 0xBE:
						printf( "RES 7" );
						RES( 7, getByteAt( reg->iy + offset ) );
						break;
					case 0xC6:
						printf( "SET %d", offset );
						SET( 0, getByteAt( reg->iy + offset ) );
						break;	
					case 0xCE:
						printf( "SET %d", offset );
						SET( 1, getByteAt( reg->iy + offset ) );
						break;
					case 0xD6:
						printf( "SET %d", offset );	
						SET( 2, getByteAt( reg->iy + offset ) );
						break;
					case 0xDE:
					        printf( "SET %d", offset );
						SET( 3, getByteAt( reg->iy + offset ) );
						break;
					case 0xE6:
						printf( "SET %d", offset );
						SET( 4, getByteAt( reg->iy + offset ) );
						break;
					case 0xEE:
						printf( "SET %d", offset );
						SET( 5, getByteAt( reg->iy + offset ) );
						break;
					case 0xF6:
						printf( "SET %d", offset );
						SET( 6, getByteAt( reg->iy + offset ) );
						break;
					case 0xFE:
						printf( "SET %d", offset );
						SET( 7, getByteAt( reg->iy + offset ) );
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
			RST( 0x38, reg->sp, reg->pc );
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

uint8_t* getByteAt( uint16_t* addrs ){
	// Return the byte at a given 16-bit memory address	
	return getByte( *addrs );	
}

uint16_t* getWordAt( uint16_t* addrs ){
	return getWord( *addrs );
}
// Read word at memory location
uint16_t readWordAt( uint16_t *addrs ){
	return *( getWord( *addrs ) );
}
uint16_t readByteAt( uint16_t *addrs ){
	return *( getByte( *addrs ) );
}
// Write byte to memory given 16-bit address
void writeByte( uint16_t addrs, uint8_t val ){
	//setByte(addrs, val);
}


