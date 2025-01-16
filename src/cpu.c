#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
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
#include "../include/memory.h"

#define BUF_SIZE 5

int t_counter = 0;

int mode;
uint16_t ix;
uint16_t iy;

// 16-Bit stack pointer and program counter 
uint16_t sp;
uint16_t pc; // AKA Instruction pointer

// IFFI (Interrupt enabled flip-flop)
int iff1;
int iff2;

// 8-Bit Z80 main general purpose z80isters
union _AF{
	uint8_t _af[2];		// Accumulater	// Flags z80ister (Set = 1, Reset = 0)
	uint16_t af;
}af;

union _BC{
	uint8_t _bc[2];		// Byte counter
	uint16_t bc;
}bc;

union _DE{
	uint8_t _de[2];
	uint16_t de;
}de;
	
union _HL{
	uint8_t _hl[2];
	uint16_t hl;
}hl;


union _ALTAF{
	uint8_t _af[2];		// Accumulater	// Flags z80ister (Set = 1, Reset = 0)
	uint16_t af;
}altaf;

union _ALTBC{
	uint8_t _bc[2];		// Byte counter
	uint16_t bc;
}altbc;

union _ALTDE{
	uint8_t _de[2];
	uint16_t de;
}altde;
	
union _ALTHL{
	uint8_t _hl[2];
	uint16_t hl;
}althl;
// 8-Bit special z80isters - Interrupt vector
union _IR{
	uint8_t _ir[2];
	uint16_t ir;
}ir;


// Declare and initialise structure of pointers to z80ister unions, this must be here not in the .h file
Z80 _z80 = {
        // Assign const pointers to main z80isters
	.a = &af._af[1], .f = &af._af[0], .af = &af.af,
	.b = &bc._bc[1], .c = &bc._bc[0], .bc = &bc.bc,
	.d = &de._de[1], .e = &de._de[0], .de = &de.de,
	.h = &hl._hl[1], .l = &hl._hl[0], .hl = &hl.hl,
	// Assign const pointers to alternative z80isters
	.alta = &altaf._af[1], .altf = &altaf._af[0], .altaf = &altaf.af,
	.altb = &altbc._bc[1], .altc = &altbc._bc[0], .altbc = &altbc.bc,
	.altd = &altde._de[1], .alte = &altde._de[0], .altde = &altde.de,
	.alth = &althl._hl[1], .altl = &althl._hl[0], .althl = &althl.hl,
	// 8-bit Special z80isters - interrupt vector and refresh z80ister
	.i = &ir._ir[1], .r = &ir._ir[0], .ir = &ir.ir,
	// 16-bit index z80isters
	.ix = &ix, .iy = &iy,
      	// 16-bit stack pointer and program pointer
	.sp = &sp, .pc = &pc,
	// IFF (Interrupt enabled flip-flop)
	.iff1 = &iff1, .iff2 = &iff2,
	// CPU Mode
	.mode = &mode
};

// Pointer to the z80ister structure
Z80* z80 = &_z80;

// IO ports *** The CPU has 256 addressable ports, using bits 0-7 of the address bus ***
// All IO routines will park data at these location for read/write by external modules

uint8_t ports[256] = {0};
static uint8_t* IOport = ports;

// List of function pointers for each of the 256 ports
// Each function (port) takes a IO status i.e. READ or WRITE and an optional address which
// can be used to use the upper-part of the address bus
uint8_t (*portPtrs[256])( int, uint8_t );

static void execute_single_byte_opcode( uint8_t *opcode );
static void execute_multi_byte_opcode( uint8_t *opcode );
static void execute_multi_byte_opcode_CB( uint8_t *opcode );
static void execute_multi_byte_opcode_DD( uint8_t *opcode );
static void execute_multi_byte_opcode_ED( uint8_t *opcode );
static void execute_multi_byte_opcode_FD( uint8_t *opcode );

// Takes a port number and maps a function to it.
void mapPort( unsigned int port, uint8_t(*func)(int, uint8_t) ){

	if( port > 255 ){
		printf("mapPort : Unable to map port, port number out of range\n");
		return;
	}

	if( func == NULL ){
		printf("mapPort : Unable to map port, func passed in is NULL\n");
		return;
	}

	portPtrs[port] = func;
}

void execute( uint8_t* opcode )
{
	printf("entered execute\n");
	//printf("0x%04X\t%02X\t", *z80->pc, totalMem[*z80->pc] );
	
	++*z80->r;

	if( *opcode == 0xCB || *opcode == 0xDD || *opcode == 0xED || *opcode == 0xFD)
	{
		//disassemble_multi_byte_opcode( opcode );
		execute_multi_byte_opcode( opcode );

	}
	else if ( *opcode >= 0x00 && *opcode <= 0xFF)
	{
		printf("executing single byte opcode\n");

		*opcode = 0x0;
		//disassemble_single_byte_opcode( opcode );
		execute_single_byte_opcode( opcode );
	}

	assert( *opcode >= 0x00 && *opcode <= 0xFF );
}

void execute_single_byte_opcode( uint8_t *opcode )
{
	printf("running instr\n");

	singleByteInstructionLookup[*opcode].func(z80);
	/*switch( *opcode )*/
	/*{*/
	/**/
	/*//	case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x47: case 0x48: case 0x49: case 0x4A: case 0x4B: case 0x4C: case 0x4D: case 0x4F:*/
	/*//	case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x57: case 0x58: case 0x59: case 0x5A: case 0x5B: case 0x5C: case 0x5D: case 0x5F: */
	/*//	case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x67: case 0x68: case 0x69: case 0x6A: case 0x6B: case 0x6C: case 0x6D: case 0x6F: */
	/*//	case 0x78: case 0x79: case 0x7A: case 0x7B: case 0x7C: case 0x7D: case 0x7F:*/
	/**/
	/*	//break;*/
	/*	case 0x00:*/
	/*		NOP();*/
	/*		break;*/
	/*	case 0x01:*/
	/*		LD16( z80->bc, getNextWord() );*/
	/*		break;*/
	/*	case 0x02:*/
	/*		LD( getByteAt( *z80->bc ) ,z80->a );*/
	/*		break;*/
	/*	case 0x03:*/
	/*		INC16( z80->bc );*/
	/*		break;*/
	/*	case 0x04:*/
	/*		INC( z80->b, z80->f );*/
	/*		break;*/
	/*	case 0x05:*/
	/*		DEC( z80->b, z80->f );*/
	/*		break;*/
	/*	case 0x06:*/
	/*		LD( z80->b, getNextByte() );*/
	/*		break;*/
	/*	case 0x07:*/
	/*		RLCA( z80->a, z80->f );*/
	/*		break;*/
	/*	case 0x08:*/
	/*		EX( z80->af, z80->altaf );*/
	/*		break;*/
	/*	case 0x09:*/
	/*		ADD16( z80->hl, z80->bc, z80->f );*/
	/*		break;*/
	/*	case 0x0A:*/
	/*		t_counter += 7;*/
	/*		LD( z80->a, getByteAt( *z80->bc ) );*/
	/*		break;*/
	/*	case 0x0B:*/
	/*		DEC16( z80->bc );*/
	/*		break;*/
	/*	case 0x0C:*/
	/*		INC( z80->c, z80->f );*/
	/*		break;*/
	/*	case 0x0D:*/
	/*		DEC( z80->c, z80->f );*/
	/*		break;*/
	/*	case 0x0E:*/
	/*		LD( z80->c, getNextByte() );*/
	/*		break;*/
	/*	case 0x0F:*/
	/*		RRCA( z80->a, z80->f );*/
	/*		break;*/
	/*	case 0x10:*/
	/*		DJNZ( getNextByte(), z80->b, z80->pc );*/
	/*		break;*/
	/*	case 0x11:*/
	/*		LD16( z80->de, getNextWord() );*/
	/*		break;*/
	/*	case 0x12:*/
	/*		LD( getByteAt( *z80->de ) ,z80->a );*/
	/*		break;*/
	/*	case 0x13:*/
	/*		INC16( z80->de );*/
	/*		break;*/
	/*	case 0x14:*/
	/*		INC( z80->d, z80->f );*/
	/*		break;*/
	/*	case 0x15:*/
	/*		DEC( z80->d, z80->f );*/
	/*		break;*/
	/*	case 0x16:*/
	/*		LD( z80->d, getNextByte() );*/
	/*		break;*/
	/*	case 0x17:*/
	/*		RLA( z80->a, z80->f );*/
	/*		break;*/
	/*	case 0x18:*/
	/*		JR( getNextByte(), z80->pc );*/
	/*		break;*/
	/*	case 0x19:*/
	/*		t_counter += 11;*/
	/*		ADD16( z80->hl, z80->de, z80->f );*/
	/*		break;*/
	/*	case 0x1A:*/
	/*		LD( z80->a, getByteAt( *z80->de ) );*/
	/*		break;*/
	/*	case 0x1B:*/
	/*		DEC16( z80->de );*/
	/*		break;*/
	/*	case 0x1C:*/
	/*		INC( z80->e, z80->f );*/
	/*		break;*/
	/*	case 0x1D:*/
	/*		DEC( z80->e, z80->f );*/
	/*		break;*/
	/*	case 0x1E:*/
	/*		LD( z80->e, getNextByte() );*/
	/*		break;*/
	/*	case 0x1F:*/
	/*		RRA( z80->a, z80->f );*/
	/*		break;*/
	/*	case 0x20:*/
	/*		t_counter += 7;*/
	/*		JRNZ( z80->pc, getNextByte(), z80->f );*/
	/*		break;*/
	/*	case 0x21:*/
	/*		t_counter += 16;*/
	/*		LD16( z80->hl, getNextWord() );*/
	/*		break;*/
	/*	case 0x22:*/
	/*		LD16( getWordAt( getNextWord() ), z80->hl );*/
	/*		break;*/
	/*	case 0x23:*/
	/*		t_counter += 6;*/
	/*		INC16( z80->hl );*/
	/*		break;*/
	/*	case 0x24:*/
	/*		INC( z80->h, z80->f );*/
	/*		break;*/
	/*	case 0x25:*/
	/*		DEC( z80->h, z80->f );*/
	/*		break;*/
	/*	case 0x26:*/
	/*		LD( z80->h, getNextByte() );*/
	/*		break;/**/
	/*	case 0x27:*/
	/*		break;*/
	/*	case 0x28:*/
	/*		t_counter += 12;*/
	/*		JRZ( z80->pc, getNextByte(), z80->f );*/
	/*		break;*/
	/*	case 0x29:*/
	/*		ADD16( z80->hl, z80->hl, z80->f );*/
	/*		break;*/
	/*	case 0x2A:*/
	/*		LD16( z80->hl, getWordAt( getNextWord() ) );*/
	/*		break;*/
	/*	case 0x2B:*/
	/*		t_counter += 6;*/
	/*		DEC16( z80->hl );*/
	/*		break;*/
	/*	case 0x2C:*/
	/*		INC( z80->l, z80->f );*/
	/*		break;*/
	/*	case 0x2D:*/
	/*		DEC( z80->l, z80->f );*/
	/*		break;*/
	/*	case 0x2E:*/
	/*		LD( z80->l, getNextByte() );*/
	/*		break;*/
	/*	case 0x2F:*/
	/*		CPL( z80->a, z80->f );*/
	/*		break;*/
	/*	case 0x30:*/
	/*		t_counter += 12;*/
	/*		JRNC( z80->pc, getNextByte(), z80->f );*/
	/*		break;*/
	/*	case 0x31:*/
	/*		LD16( z80->hl, getNextWord() );*/
	/*		break;*/
	/*	case 0x32:*/
	/*		LD( getByteAt( *( getNextWord() ) ) ,z80->a );*/
	/*		break;*/
	/*	case 0x33:*/
	/*		INC16( z80->sp );*/
	/*		break;*/
	/*	case 0x34:*/
	/*		INC( getByteAt( *z80->hl ), z80->f );*/
	/*		break;*/
	/*	case 0x35:*/
	/*		t_counter += 6;*/
	/*		DEC( getByteAt( *z80->hl ), z80->f );*/
	/*		break;*/
	/*	case 0x36:*/
	/*		t_counter += 7;*/
	/*		LD( getByteAt( *z80->hl ), getNextByte() );*/
	/*		break;*/
	/*	case 0x37:*/
	/*		SCF( z80->f );*/
	/*		break;*/
	/*	case 0x38:*/
	/*		JRC( z80->pc, getNextByte(), z80->f );*/
	/*		break;*/
	/*	case 0x39:*/
	/*		ADD16( z80->hl, z80->sp, z80->f );*/
	/*		break;*/
	/*	case 0x3A:*/
	/*		LD( z80->a, getByteAt( *(getNextWord()) ) );*/
	/*		break;*/
	/*	case 0x3B:*/
	/*		DEC16( z80->sp );*/
	/*		break;*/
	/*	case 0x3C:*/
	/*		INC( z80->a, z80->f );*/
	/*		break;*/
	/*	case 0x3D:*/
	/*		DEC( z80->a, z80->f );*/
	/*		break;*/
	/*	case 0x3E:*/
	/*		LD( z80->a, getNextByte() );*/
	/*		t_counter += 13;*/
	/*		break;*/
	/*	case 0x3F:*/
	/*		CCF( z80->f );*/
	/*		break;*/
	/*	case 0x40:*/
	/*		t_counter += 4;*/
	/*		LD( z80->b, z80->b );*/
	/*		break;*/
	/*	case 0x41:*/
	/*		t_counter += 4;*/
	/*		LD( z80->b, z80->c );*/
	/*		break;*/
	/*	case 0x42:*/
	/*		t_counter += 4;*/
	/*		LD( z80->b, z80->d );*/
	/*		break;*/
	/*	case 0x43:*/
	/*		t_counter += 4;*/
	/*		LD( z80->b, z80->e );*/
	/*		break;*/
	/*	case 0x44:*/
	/*		t_counter += 4;*/
	/*		LD( z80->b, z80->h );*/
	/*		break;*/
	/*	case 0x45:*/
	/*		t_counter += 4;*/
	/*		LD( z80->b, z80->l );*/
	/*		break;*/
	/*	case 0x46:*/
	/*		t_counter += 4;*/
	/*		LD( z80->b, getByteAt( *z80->hl ) );*/
	/*		break;*/
	/*	case 0x47:*/
	/*		t_counter += 4;*/
	/*		LD( z80->b, z80->a );*/
	/*		break;*/
	/*	case 0x48:*/
	/*		t_counter += 4;*/
	/*		LD( z80->c, z80->b );*/
	/*		break;*/
	/*	case 0x49:*/
	/*		t_counter += 4;*/
	/*		LD( z80->c, z80->c );*/
	/*		break;*/
	/*	case 0x4A:*/
	/*		t_counter += 4;*/
	/*		LD( z80->c, z80->d );*/
	/*		break;*/
	/*	case 0x4B:*/
	/*		t_counter += 4;*/
	/*		LD( z80->c, z80->e );*/
	/*		break;*/
	/*	case 0x4C:*/
	/*		t_counter += 4;*/
	/*		LD( z80->c, z80->h );*/
	/*		break;*/
	/*	case 0x4D:*/
	/*		t_counter += 4;*/
	/*		LD( z80->c, z80->l );*/
	/*		break;*/
	/*	case 0x4E:*/
	/*		t_counter += 7;*/
	/*		LD( z80->c, getByteAt( *z80->hl ) );*/
	/*		break;*/
	/*	case 0x4F:*/
	/*		t_counter += 4;*/
	/*		LD( z80->c, z80->a );*/
	/*		break;*/
	/*	case 0x50:*/
	/*		t_counter += 4;*/
	/*		LD( z80->d, z80->b );*/
	/*		break;*/
	/*	case 0x51:*/
	/*		t_counter += 4;*/
	/*		LD( z80->d, z80->c );*/
	/*		break;*/
	/*	case 0x52:*/
	/*		t_counter += 4;*/
	/*		LD( z80->d, z80->d );*/
	/*		break;*/
	/*	case 0x53:*/
	/*		t_counter += 4;*/
	/*		LD( z80->d, z80->e );*/
	/*		break;*/
	/*	case 0x54:*/
	/*		t_counter += 4;*/
	/*		LD( z80->d, z80->h );*/
	/*		break;*/
	/*	case 0x55:*/
	/*		t_counter += 4;*/
	/*		LD( z80->d, z80->l );*/
	/*		break;*/
	/*	case 0x56:*/
	/*		t_counter += 7;*/
	/*		LD( z80->d, getByteAt( *z80->hl ) );*/
	/*		break;*/
	/*	case 0x57:*/
	/*		t_counter += 4;*/
	/*		LD( z80->d, z80->a );*/
	/*		break;*/
	/*	case 0x58:*/
	/*		t_counter += 4;*/
	/*		LD( z80->e, z80->b );*/
	/*		break;*/
	/*	case 0x59:*/
	/*		t_counter += 4;*/
	/*		LD( z80->e, z80->c );*/
	/*		break;*/
	/*	case 0x5A:*/
	/*		t_counter += 4;*/
	/*		LD( z80->e, z80->d );*/
	/*		break;*/
	/*	case 0x5B:*/
	/*		t_counter += 4;*/
	/*		LD( z80->e, z80->e );*/
	/*		break;*/
	/*	case 0x5C:*/
	/*		t_counter += 4;*/
	/*		LD( z80->e, z80->h );*/
	/*		break;*/
	/*	case 0x5D:*/
	/*		t_counter += 4;*/
	/*		LD( z80->e, z80->l );*/
	/*		break;*/
	/*	case 0x5E:*/
	/*		t_counter += 7;*/
	/*		LD( z80->e, getByteAt( *z80->hl ) );*/
	/*		break;*/
	/*	case 0x5F:*/
	/*		t_counter += 4;*/
	/*		LD( z80->e, z80->a );*/
	/*		break;*/
	/*	case 0x60:*/
	/*		t_counter += 4;*/
	/*		LD( z80->h, z80->b );*/
	/*		break;*/
	/*	case 0x61:*/
	/*		t_counter += 4;*/
	/*		LD( z80->h, z80->c );*/
	/*		break;*/
	/*	case 0x62:*/
	/*		t_counter += 4;*/
	/*		LD( z80->h, z80->d );*/
	/*		break;*/
	/*	case 0x63:*/
	/*		t_counter += 4;*/
	/*		LD( z80->h, z80->e );*/
	/*		break;*/
	/*	case 0x64:*/
	/*		t_counter += 4;*/
	/*		LD( z80->h, z80->h );*/
	/*		break;*/
	/*	case 0x65:*/
	/*		t_counter += 4;*/
	/*		LD( z80->h, z80->l );*/
	/*		break;*/
	/*	case 0x66:*/
	/*		LD( z80->h, getByteAt( *z80->hl ) );*/
	/*		t_counter += 7;*/
	/*		break;*/
	/*	case 0x67:*/
	/*		t_counter += 4;*/
	/*		LD( z80->h, z80->a );*/
	/*		break;*/
	/*	case 0x68:*/
	/*		t_counter += 4;*/
	/*		LD( z80->l, z80->b );*/
	/*		break;*/
	/*	case 0x69:*/
	/*		t_counter += 4;*/
	/*		LD( z80->l, z80->c );*/
	/*		break;*/
	/*	case 0x6A:*/
	/*		t_counter += 4;*/
	/*		LD( z80->l, z80->d );*/
	/*		break;*/
	/*	case 0x6B:*/
	/*		t_counter += 4;*/
	/*		LD( z80->l, z80->e );*/
	/*		break;*/
	/*	case 0x6C:*/
	/*		t_counter += 4;*/
	/*		LD( z80->l, z80->h );*/
	/*		break;*/
	/*	case 0x6D:*/
	/*		t_counter += 4;*/
	/*		LD( z80->l, z80->l );*/
	/*		break;*/
	/*	case 0x6E:*/
	/*		LD( z80->l, getByteAt( *z80->hl ) );*/
	/*		t_counter += 7;*/
	/*		break;*/
	/*	case 0x6F:*/
	/*		t_counter += 4;*/
	/*		LD( z80->l, z80->a );*/
	/*		break;*/
	/*	case 0x70:*/
	/*		t_counter += 7;*/
	/*		LD( getByteAt( *z80->hl ) ,z80->b );*/
	/*		break;*/
	/*	case 0x71:*/
	/*		t_counter += 7;*/
	/*		LD( getByteAt( *z80->hl ), z80->c );*/
	/*		break;*/
	/*	case 0x72:*/
	/*		t_counter += 7;*/
	/*		LD( getByteAt( *z80->hl ), z80->d );*/
	/*		break;*/
	/*	case 0x73:*/
	/*		t_counter += 7;*/
	/*		LD( getByteAt( *z80->hl ), z80->e );*/
	/*		break;*/
	/*	case 0x74:*/
	/*		t_counter += 7;*/
	/*		LD( getByteAt( *z80->hl ), z80->h );*/
	/*		break;*/
	/*	case 0x75:*/
	/*		t_counter += 7;*/
	/*		LD( getByteAt( *z80->hl ), z80->l );*/
	/*		break;/**/
	/*	case 0x76:*/
	/*		HALT*/
	/*		break;*/
	/*	case 0x77:*/
	/*		t_counter += 7;*/
	/*		LD( getByteAt( *z80->hl ) ,z80->a );*/
	/*		break;*/
	/*	case 0x78:*/
	/*		t_counter += 4;*/
	/*		LD( z80->a, z80->b );*/
	/*		break;*/
	/*	case 0x79:*/
	/*		t_counter += 4;*/
	/*		LD( z80->a, z80->c );*/
	/*		break;*/
	/*	case 0x7A:*/
	/*		t_counter += 4;*/
	/*		LD( z80->a, z80->d );*/
	/*		break;*/
	/*	case 0x7B:*/
	/*		LD( z80->a, z80->e );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0x7C:*/
	/*		LD( z80->a, z80->h );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0x7D:*/
	/*		LD( z80->a, z80->l );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0x7E:*/
	/*		t_counter += 7;*/
	/*		LD( z80->a, getByteAt( *z80->hl ) );*/
	/*		break;*/
	/*	case 0x7F:*/
	/*		LD( z80->a, z80->a );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0x80:*/
	/*		ADD( z80->a, z80->b, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0x81:*/
	/*		ADD( z80->a, z80->c, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0x82:*/
	/*		ADD( z80->a, z80->d, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0x83:*/
	/*		t_counter += 4;*/
	/*		ADD( z80->a, z80->e, z80->f );*/
	/*		break;*/
	/*	case 0x84:*/
	/*		t_counter += 4;*/
	/*		ADD( z80->a, z80->h, z80->f );*/
	/*		break;*/
	/*	case 0x85:*/
	/*		t_counter += 4;*/
	/*		ADD( z80->a, z80->l, z80->f );*/
	/*		break;*/
	/*	case 0x86:*/
	/*		ADD( z80->a, getByteAt( *z80->hl ), z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0x87:*/
	/*		ADD( z80->a, z80->a, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0x88:*/
	/*		ADC( z80->a, z80->b, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0x89:*/
	/*		ADC( z80->a, z80->c, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0x8A:*/
	/*		ADC( z80->a, z80->d, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0x8B:*/
	/*		ADC( z80->a, z80->e, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0x8C:*/
	/*		ADC( z80->a, z80->h, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0x8D:*/
	/*		ADC( z80->a, z80->l, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0x8E:*/
	/*		ADC( z80->a, getByteAt( *z80->hl ), z80->f );*/
	/*		break;*/
	/*	case 0x8F:*/
	/*		ADC( z80->a, z80->a, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0x90:*/
	/*		SUB( z80->a, z80->b, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0x91:*/
	/*		SUB( z80->a, z80->c, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0x92:*/
	/*		SUB( z80->a, z80->d, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0x93:*/
	/*		SUB( z80->a, z80->e, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0x94:*/
	/*		SUB( z80->a, z80->h, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0x95:*/
	/*		SUB( z80->a, z80->l, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0x96:*/
	/*		SUB( z80->a, getByteAt( *z80->hl ), z80->f );*/
	/*		break;*/
	/*	case 0x97:*/
	/*		SUB( z80->a, z80->a, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0x98:*/
	/*		SBC( z80->a, z80->b, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0x99:*/
	/*		SBC( z80->a, z80->c, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0x9A:*/
	/*		SBC( z80->a, z80->d, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0x9B:*/
	/*		SBC( z80->a, z80->e, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0x9C:*/
	/*		SBC( z80->a, z80->h, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0x9D:*/
	/*		SBC( z80->a, z80->l, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0x9E:*/
	/*		SBC( z80->a, getByteAt( *z80->hl ), z80->f );*/
	/*		break;*/
	/*	case 0x9F:*/
	/*		SBC( z80->a, z80->a, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0xA0:*/
	/*		AND( z80->a, z80->b, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0xA1:*/
	/*		AND( z80->a, z80->c, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0xA2:*/
	/*		AND( z80->a, z80->d, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0xA3:*/
	/*		AND( z80->a, z80->e, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0xA4:*/
	/*		AND( z80->a, z80->h, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0xA5:*/
	/*		AND( z80->a, z80->l, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0xA6:*/
	/*		AND( z80->a, getByteAt( *z80->hl ), z80->f );*/
	/*		break;*/
	/*	case 0xA7:*/
	/*		t_counter += 4;*/
	/*		AND( z80->a, z80->a, z80->f );*/
	/*		break;*/
	/*	case 0xA8:*/
	/*		XOR( z80->a, z80->b, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0xA9:*/
	/*		XOR( z80->a, z80->c, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0xAA:*/
	/*		XOR( z80->a, z80->d, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0xAB:*/
	/*		XOR( z80->a, z80->e, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0xAC:*/
	/*		XOR( z80->a, z80->h, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0xAD:*/
	/*		XOR( z80->a, z80->l, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0xAE:*/
	/*		XOR( z80->a, getByteAt( *z80->hl ), z80->f );*/
	/*		break;*/
	/*	case 0xAF:*/
	/*		XOR( z80->a, z80->a, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0xB0:*/
	/*		OR( z80->a, z80->b, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0xB1:*/
	/*		OR( z80->a, z80->c, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0xB2:*/
	/*		OR( z80->a, z80->d, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0xB3:*/
	/*		OR( z80->a, z80->e, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0xB4:*/
	/*		OR( z80->a, z80->h, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0xB5:*/
	/*		OR( z80->a, z80->l, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0xB6:*/
	/*		OR( z80->a, getByteAt( *z80->hl ), z80->f );*/
	/*		break;*/
	/*	case 0xB7:*/
	/*		t_counter += 4;*/
	/*		OR( z80->a, z80->a, z80->f );*/
	/*		break;*/
	/*	case 0xB8:*/
	/*		t_counter += 4;*/
	/*		CP( z80->a, z80->b, z80->f );*/
	/*		break;*/
	/*	case 0xB9:*/
	/*		CP( z80->a, z80->c, z80->f );*/
	/*		t_counter += 4;*/
	/*		break;*/
	/*	case 0xBA:*/
	/*		t_counter += 4;*/
	/*		CP( z80->a, z80->d, z80->f );*/
	/*		break;*/
	/*	case 0xBB:*/
	/*		t_counter += 4;*/
	/*		CP( z80->a, z80->e, z80->f );*/
	/*		break;*/
	/*	case 0xBC:*/
	/*		t_counter += 4;*/
	/*		CP( z80->a, z80->h, z80->f );*/
	/*		break;*/
	/*	case 0xBD:*/
	/*		t_counter += 4;*/
	/*		CP( z80->a, z80->l, z80->f );*/
	/*		break;*/
	/*	case 0xBE:*/
	/*		CP( z80->a, getByteAt( *z80->hl ), z80->f );*/
	/*		break;*/
	/*	case 0xBF:*/
	/*		t_counter += 4;*/
	/*		CP( z80->a, z80->a, z80->f );*/
	/*		break;*/
	/*	case 0xC0:*/
	/*		t_counter += 11;*/
	/*		RETNZ( z80->pc, getWordAt( z80->sp ), z80->sp, z80->f );*/
	/*		break;*/
	/*	case 0xC1:*/
	/*		t_counter += 10;*/
	/*		POP( getWordAt( z80->sp ) ,z80->sp, z80->bc );*/
	/*		break;*/
	/*	case 0xC2:*/
	/*		JPNZ( z80->pc, getNextWord(), z80->f );*/
	/*		break;*/
	/*	case 0xC3:*/
	/*		JP( z80->pc, getNextWord() );*/
	/*		break;*/
	/*	case 0xC4:*/
	/*		CALLNZ( getNextWord(), getWordAt( z80->sp ), z80->sp, z80->pc, z80->f );*/
	/*		break;*/
	/*	case 0xC5:*/
	/*		t_counter += 11;*/
	/*		PUSH( getWordAt( z80->sp ), z80->sp, z80->bc );*/
	/*		break;*/
	/*	case 0xC6:*/
	/*		ADD( z80->a, getNextByte(), z80->f );*/
	/*		break;*/
	/*	case 0xC7:*/
	/*		RST( 0x00, getWordAt( z80->sp ), z80->sp, z80->pc );*/
	/*		break;*/
	/*	case 0xC8:*/
	/*		t_counter += 11;*/
	/*		RETZ( z80->pc, getWordAt( z80->sp ), z80->sp, z80->f );*/
	/*		break;*/
	/*	case 0xC9:*/
	/*		t_counter += 10;*/
	/*		RET( z80->pc, getWordAt( z80->sp ), z80->sp );*/
	/*		break;*/
	/*	case 0xCA:*/
	/*		t_counter += 12;*/
	/*		JPZ( z80->pc, getNextWord(), z80->f );*/
	/*		break;*/
	/*	case 0xCB:*/
	/*		execute_multi_byte_opcode(opcode);*/
	/*		break;*/
	/*	case 0xCC:*/
	/*		CALLZ( getNextWord(), getWordAt( z80->sp ), z80->sp, z80->pc, z80->f );*/
	/*		break;*/
	/*	case 0xCD:*/
	/*		t_counter += 17;*/
	/*		CALL( getNextWord(), getWordAt( z80->sp ), z80->sp, z80->pc );*/
	/*		break;*/
	/*	case 0xCE:*/
	/*		ADC( z80->a, getNextByte(), z80->f );*/
	/*		break;*/
	/*	case 0xCF:*/
	/*		RST( 0x08, getWordAt( z80->sp ), z80->sp, z80->pc );*/
	/*		break;*/
	/*	case 0xD0:*/
	/*		RETNC( z80->pc, getWordAt( z80->sp ), z80->sp, z80->f );*/
	/*		break;*/
	/*	case 0xD1:*/
	/*		POP( getWordAt( z80->sp ) ,z80->sp, z80->de );*/
	/*		break;*/
	/*	case 0xD2:*/
	/*		JPNC( z80->pc, getNextWord(), z80->f );*/
	/*		break;*/
	/*	case 0xD3:*/
	/*		OUTA( &IOport[ *( getNextByte() ) ], z80->a );*/
	/*		break;*/
	/*	case 0xD4:*/
	/*		CALLNC( getNextWord(), getWordAt( z80->sp ), z80->sp, z80->pc, z80->f  );*/
	/*		break;*/
	/*	case 0xD5:*/
	/*		PUSH( getWordAt( z80->sp ), z80->sp, z80->de );*/
	/*		break;*/
	/*	case 0xD6:*/
	/*		SUB( z80->a, getNextByte(), z80->f );*/
	/*		break;*/
	/*	case 0xD7:*/
	/*		RST( 0x10, getWordAt( z80->sp ), z80->sp, z80->pc );*/
	/*		break;*/
	/*	case 0xD8:*/
	/*		RETC( z80->pc, getWordAt( z80->sp ), z80->sp, z80->f );*/
	/*		break;*/
	/*	case 0xD9:*/
	/*		t_counter += 4;*/
	/*		EXX( z80->bc, z80->de, z80->hl, z80->altbc, z80->altde, z80->althl );*/
	/*		break;*/
	/*	case 0xDA:*/
	/*		JPC( z80->pc, getNextWord(), z80->f );*/
	/*		break;*/
	/*	case 0xDB:*/
	/*		INA( z80->a, readPort(0x00FE) );*/
	/*		break;*/
	/*	case 0xDC:*/
	/*		CALLC( getNextWord(), getWordAt( z80->sp ), z80->sp, z80->pc, z80->f  );*/
	/*		break;*/
	/*	case 0xDD: /* IX z80isters */
	/*		execute_multi_byte_opcode_DD( opcode );*/
	/*	case 0xDE:*/
	/*		SBC( z80->a, getNextByte(), z80->f );*/
	/*		break;*/
	/*	case 0xDF:*/
	/*		RST( 0x18, getWordAt( z80->sp ), z80->sp, z80->pc );*/
	/*		break;*/
	/*	case 0xE0:*/
	/*		RETPO( z80->pc, getWordAt( z80->sp ), z80->sp, z80->f );*/
	/*		break;*/
	/*	case 0xE1:*/
	/*		POP( getWordAt( z80->sp ) ,z80->sp, z80->hl );*/
	/*		break;*/
	/*	case 0xE2:*/
	/*		JPPO( z80->pc, getNextWord(), z80->f );*/
	/*		break;*/
	/*	case 0xE3:*/
	/*		EX( getWordAt( z80->sp ), z80->hl );*/
	/*		break;*/
	/*	case 0xE4:*/
	/*		CALLPO( getNextWord(), getWordAt( z80->sp ), z80->sp, z80->pc, z80->f  );*/
	/*		break;*/
	/*	case 0xE5:*/
	/*		PUSH( getWordAt( z80->sp ), z80->sp, z80->hl );*/
	/*		break;*/
	/*	case 0xE6:*/
	/*		AND( z80->a, getNextByte(), z80->f );*/
	/*		break;*/
	/*	case 0xE7:*/
	/*		RST( 0x20, getWordAt( z80->sp ), z80->sp, z80->pc );*/
	/*		break;*/
	/*	case 0xE8:*/
	/*		RETPE( z80->pc, getWordAt( z80->sp ), z80->sp, z80->f  );*/
	/*		break;*/
	/*	case 0xE9:*/
	/*		JP( z80->pc, z80->hl );*/
	/*		break;*/
	/*	case 0xEA:*/
	/*		JPPE( z80->pc, getNextWord(), z80->f );*/
	/*		break;*/
	/*	case 0xEB:*/
	/*		EX( z80->de, z80->hl );*/
	/*		break;*/
	/*	case 0xEC:*/
	/*		CALLPE( getNextWord(), getWordAt( z80->sp ), z80->sp, z80->pc, z80->f  );*/
	/*		break;*/
	/*	case 0xED:*/
	/*		execute_multi_byte_opcode_ED( opcode );*/
	/*		break;*/
	/*	case 0xEE:*/
	/*		XOR( z80->a, getNextByte(), z80->f );*/
	/*		break;*/
	/*	case 0xEF:*/
	/*		RST( 0x28, getWordAt( z80->sp ), z80->sp, z80->pc );*/
	/*		break;*/
	/*	case 0xF0:*/
	/*		RETP( z80->pc, getWordAt( z80->sp ), z80->sp, z80->f  );*/
	/*		break;*/
	/*	case 0xF1:*/
	/*		POP( getWordAt( z80->sp ) ,z80->sp, z80->af );*/
	/*		break;*/
	/*	case 0xF2:*/
	/*		JPP( z80->pc, getNextWord(), z80->f  );*/
	/*		break;*/
	/*	case 0xF3:*/
	/*		DI( z80->iff1, z80->iff2 );*/
	/*		break;*/
	/*	case 0xF4:*/
	/*		CALLP( getNextWord(), getWordAt( z80->sp ), z80->sp, z80->pc, z80->f );*/
	/*		break;*/
	/*	case 0xF5:*/
	/*		PUSH( getWordAt( z80->sp ), z80->sp, z80->af );*/
	/*		break;*/
	/*	case 0xF6:*/
	/*		OR( z80->a, getNextByte(), z80->f );*/
	/*		break;*/
	/*	case 0xF7:*/
	/*		RST( 0x30, getWordAt( z80->sp ), z80->sp, z80->pc );*/
	/*		break;*/
	/*	case 0xF8:*/
	/*		RETM( z80->pc, getWordAt( z80->sp ), z80->sp, z80->f );*/
	/*		break;*/
	/*	case 0xF9:*/
	/*		LD16( z80->sp, z80->hl );*/
	/*		break;*/
	/*	case 0xFA:*/
	/*		JPM( z80->pc, getNextWord(), z80->f );*/
	/*		break;*/
	/*	case 0xFB:*/
	/*		EI( z80->iff1, z80->iff2 );*/
	/*		break;*/
	/*	case 0xFC:*/
	/*		CALLM( getNextWord(), getWordAt( z80->sp ), z80->sp, z80->pc, z80->f  );*/
	/*		break;*/
	/*	case 0xFD:*/
	/*		execute_multi_byte_opcode_FD( opcode );*/
	/*	default:*/
	/*		break;*/
	/*}*/
}


void execute_multi_byte_opcode( uint8_t *opcode )
{
	switch( *opcode )
	{
		case 0xCB:
			execute_multi_byte_opcode_CB( opcode );
			break;
		case 0xDD:
			execute_multi_byte_opcode_DD( opcode );
			break;
		case 0xED:
			execute_multi_byte_opcode_ED( opcode );
			break;
		default:
			break;
	}
}

void execute_multi_byte_opcode_CB( uint8_t *opcode )
{
	switch( *( getNextByte() ) )
	{
		case 0x00:
			RLC( z80->b, z80->f );
			break;
		case 0x01:
			RLC( z80->c, z80->f );
			break;
		case 0x02:
			RLC( z80->d, z80->f );
			break;
		case 0x03:
			RLC( z80->e, z80->f );
			break;
		case 0x04:
			RLC( z80->h, z80->f );
			break;
		case 0x05:
			RLC( z80->l, z80->f );
			break;
		case 0x06:
			RLC( getByteAt( *z80->hl ), z80->f );
			break;
		case 0x07:
			RLC( z80->a, z80->f );
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
			SRL( z80->h, z80->f );
			break;
		case 0x3D:
			SRL( z80->l, z80->f );
			break;
		case 0x3E:
			break;
		case 0x3F:
			break;
		case 0x40:
			BIT( 0, z80->b, z80->f );
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
			BIT( 0, getByteAt( *z80->hl ), z80->f );
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
			BIT( 1, getByteAt( *z80->hl ), z80->f );
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
			BIT( 2, getByteAt( *z80->hl ), z80->f );
			break;
		case 0x57:
			break;
		case 0x58:
			break;
		case 0x59:
			break;
		case 0x5A:
			BIT( 3, z80->d, z80->f );
			break;
		case 0x5B:
			break;
		case 0x5C:
			break;
		case 0x5D:
			break;
		case 0x5E:
			BIT( 3, getByteAt( *z80->hl ), z80->f );
			break;
		case 0x5F:
			break;
		case 0x60:
			BIT( 4, z80->b, z80->f );
			break;
		case 0x61:
			BIT( 4, z80->c, z80->f );
			break;
		case 0x62:
			BIT( 4, z80->d, z80->f );
			break;
		case 0x63:
			BIT( 4, z80->e, z80->f );
			break;
		case 0x64:
			BIT( 4, z80->h, z80->f );
			break;
		case 0x65:
			BIT( 4, z80->l, z80->f );
			break;
		case 0x66:
			BIT( 4, getByteAt( *z80->hl ), z80->f );
			break;
		case 0x67:
			BIT( 4, z80->a, z80->f );
			break;
		case 0x6E:
			BIT( 5, getByteAt( *z80->hl ), z80->f );
			break;
		case 0x76:
			BIT( 6, getByteAt( *z80->hl ), z80->f );
			break;
		case 0x7E:
			BIT( 7, getByteAt( *z80->hl ), z80->f );
			break;
		case 0x86:
			RES( 0, getByteAt( *z80->hl ) );
			break;
		case 0x8E:
			RES( 1, getByteAt( *z80->hl ) );
			break;
		case 0x96:
			RES( 2, getByteAt( *z80->hl ) );
			break;
		case 0x9E:
			RES( 3, getByteAt( *z80->hl ) );
			break;
		case 0xA6:
			RES( 4, getByteAt( *z80->hl ) );
			break;
		case 0xAE:
			RES( 5, getByteAt( *z80->hl ) );
			break;
		case 0xB6:
			RES( 6, getByteAt( *z80->hl ) );
			break;
		case 0xBC:
			RES( 7, z80->h );
			break;
		case 0xBE:
			RES( 7, getByteAt( *z80->hl ) );
			break;
		case 0xC6:
			SET( 0, getByteAt( *z80->hl ) );
			break;
		case 0xCE:
			SET( 1, getByteAt( *z80->hl ) );
			break;
		case 0xD6:
			SET( 2, getByteAt( *z80->hl ) );
			break;
		case 0xDE:
			SET( 3, getByteAt( *z80->hl ) );
			break;
		case 0xE6:
			SET( 4, getByteAt( *z80->hl ) );
			break;
		case 0xEE:
			SET( 5, getByteAt( *z80->hl ) );
			break;
		case 0xF6:
			SET( 6, getByteAt( *z80->hl ) );
			break;
		case 0xFD:
			SET( 7, z80->l );
			break;
		case 0xFE:
			SET( 7, getByteAt( *z80->hl ) );
			break;
		default:
			break;
	}
}


void execute_multi_byte_opcode_DD( uint8_t *opcode )
{
	switch( *( getNextByte() ) ){
		case 0x09:
			printf( "ADD IX + BC" );
			ADD16( z80->ix, z80->bc ,z80->f );
			break;
		case 0x21:
			printf( "LD IX,+%X", readNextWord() );
			LD16( z80->ix, getNextWord() );
			break;
		case 0x22:
			printf( "LD (%X),IX", readNextWord() );
			LD16( getWordAt( getNextWord() ), z80->ix );
			break;
		case 0x23:
			printf( "INC IX" );
			INC16( z80->ix );
			break;
		case 0x2A:
			printf( "LD IX(%X)", readNextWord() );
			LD16( z80->ix, getWordAt( getNextWord() ) );
			break;
		case 0x2B:
			printf( "DEC IX" );
			DEC16( z80->ix );
			break;
		case 0x34:
			printf( "INC IX+%X",(int8_t) readNextByte() );
			INC( getByteAt( *z80->ix +(int8_t) *( getNextByte() ) ), z80->f );
			break;
		case 0x35:
			printf( "DEC IX +%X",(int8_t) readNextByte() );
			DEC( getByteAt( *z80->ix +(int8_t) *( getNextByte() ) ), z80->f );
			break;
		case 0x36:
			printf( "LD (IX+%X),+%X",(int8_t) readNextByte(), readNextByte() + 1 ); // BROKEN second param is incorrect
			LD( getByteAt( *z80->ix +(int8_t) *( getNextByte() ) ), getNextByte() );
			break;
		case 0x46:
			printf( "LD B,(IX+%X)",(int8_t) readNextByte() );
			LD( z80->b, getByteAt( *z80->ix +(int8_t) *(getNextByte()) ) );
			break;
		case 0x4E:
			printf( "LD C,(IX+%X)",(int8_t) readNextByte() );
			LD( z80->c, getByteAt( *z80->ix +(int8_t) *(getNextByte() ) ) );
			break;
		case 0x5E:
			printf( "LD D,(IX+%X)",(int8_t) readNextByte() );
			LD( z80->d, getByteAt( *z80->ix +(int8_t) *(getNextByte() ) ) );
			break;
		case 0x56:
			printf( "LD E,(IX+%X)",(int8_t) readNextByte() );
			LD( z80->e, getByteAt( *z80->ix + (int8_t)*(getNextByte() ) ) );
			break;
		case 0x66:
			printf( "LD H,(IX+%X)",(int8_t) readNextByte() );
			LD( z80->h, getByteAt( *z80->ix +(int8_t) *(getNextByte() ) ) );
			break;
		case 0x6E:
			printf( "LD L,(IX+%X)",(int8_t) readNextByte() );
			LD( z80->l, getByteAt( *z80->ix +(int8_t) *( getNextByte() ) ) );
			break;
		case 0x70:
			printf( "LD (IX+%X),B",(int8_t) readNextByte() );
			LD( getByteAt( *z80->ix + (int8_t)*( getNextByte() ) ), z80->b );
			break;
		case 0x71:
			printf( "LD (IX+%X),C", (int8_t)readNextByte() );
			LD( getByteAt( *z80->ix + (int8_t)*( getNextByte() ) ), z80->c );
			break;
		case 0x72:
			printf( "LD (IX+%X),D", (int8_t)readNextByte() );
			LD( getByteAt( *z80->ix + (int8_t)*( getNextByte() ) ), z80->d );
			break;
		case 0x73:
			printf( "LD (IX+%X),E", (int8_t)readNextByte() );
			LD( getByteAt( *z80->ix + (int8_t)*( getNextByte() ) ), z80->e );
			break;
		case 0x74:
			printf( "LD (IX+%X),H", (int8_t)readNextByte() );
			LD( getByteAt( *z80->ix + (int8_t)*( getNextByte() ) ), z80->h );
			break;
		case 0x75:
			printf( "LD (IX+%X),L", (int8_t)readNextByte() );
			LD( getByteAt( *z80->ix + (int8_t)*( getNextByte() ) ), z80->l );
			break;
		case 0x77:
			printf( "LD (IX+%X),A", (int8_t)readNextByte() );
			LD( getByteAt( *z80->ix + (int8_t)*( getNextByte() ) ), z80->a );
			break;
		case 0x7E:
			printf( "LD A,(IX+%X)", (int8_t)readNextByte() );
			LD( z80->a, getByteAt( *z80->ix + (int8_t)*( getNextByte() ) ) );
			break;
		case 0x86:
			printf( "ADD (IX+%X)", (int8_t)readNextByte() );
			ADD( z80->a, getByteAt( *z80->ix + (int8_t)*( getNextByte() ) ), z80->f );
			break;
		case 0x8E:
			printf( "ADC (IX+%X)", (int8_t)readNextByte() );
			ADC( z80->a, getByteAt( *z80->ix + (int8_t)*( getNextByte() ) ), z80->f );
			break;
		case 0x96:
			printf( "SUB (IX+%X)", (int8_t)readNextByte() );
			SUB( z80->a, getByteAt( *z80->ix + (int8_t)*( getNextByte() ) ), z80->f );
			break;
		case 0xA6:
			printf( "AND A (IX+%X)",(int8_t)readNextByte() );
			AND( z80->a, getByteAt( *z80->ix + (int8_t)*( getNextByte() ) ), z80->f );
			break;
		case 0xAE:
			printf( "XOR (IX+%X)", (int8_t)readNextByte() );
			XOR( z80->a, getByteAt( *z80->ix + (int8_t)*( getNextByte() ) ), z80->f );
			break;
		case 0xB6:
			printf( "OR A,(IX+%X)", (int8_t)readNextByte() );
			OR( z80->a, getByteAt( *z80->ix + (int8_t)*( getNextByte() ) ), z80->f );
			break;
		case 0xBE:
			printf( "CP (IX+%X)",(int8_t) readNextByte() );
			CP( z80->a, getByteAt( *z80->ix +(int8_t) *( getNextByte() ) ) ,z80->f );
			break;
		case 0xCB:
		uint8_t offset = *( getNextByte() );
			switch( *( getNextByte() ) ){
				case 0x06:
					printf( "RLC(IX+%X)", offset );
					RLC( getByteAt( *z80->ix + (int8_t) *( getNextByte() ) ), z80->f );
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
					BIT( 0, getByteAt( *z80->ix + (int8_t)offset ), z80->f );
					break;
				case 0x4E:
					printf( "BIT 1, (IX+%X)", (int8_t)offset );
					BIT( 1, getByteAt( *z80->ix + (int8_t)offset ), z80->f );
					break;
				case 0x56:
					printf( "BIT 2, (IX+%X)", (int8_t)offset );
					BIT( 2, getByteAt( *z80->ix + (int8_t)offset ), z80->f );
					break;
				case 0x5E:
					printf( "BIT 3, (IX+%X)", (int8_t)offset );
					BIT( 3, getByteAt( *z80->ix + (int8_t)offset ), z80->f );
					break;
				case 0x66:
					printf( "BIT 4, (IX+%X)", (int8_t)offset );
					BIT( 4, getByteAt( *z80->ix + (int8_t)offset ), z80->f );
					break;
				case 0x6E:
					printf( "BIT 5, (IX+%X)", (int8_t)offset );
					BIT( 5, getByteAt( *z80->ix + (int8_t)offset ), z80->f );
					break;
				case 0x76:
					printf( "BIT 6, (IX+%X)", (int8_t)offset );
					BIT( 6, getByteAt( *z80->ix + (int8_t)offset ), z80->f );
					break;
				case 0x7E:
					printf( "BIT 7, (IX+%X)", (int8_t)offset );
					BIT( 7, getByteAt( *z80->ix + (int8_t)offset ), z80->f );
					break;
				case 0x86:
					printf( "RES 0, (IX+%X)", (int8_t)offset );
					RES( 0, getByteAt( *z80->ix + (int8_t)offset ) );
					break;
				case 0x8E:
					printf( "RES 1, (IX+%X)", (int8_t)offset );
					RES( 1, getByteAt( *z80->ix + (int8_t)offset ) );
					break;
				case 0x96:
					printf( "RES 2, (IX+%X)", (int8_t)offset );
					RES( 2, getByteAt( *z80->ix + (int8_t)offset ) );
					break;
				case 0x9E:
					printf( "RES 3, (IX+%X)", (int8_t)offset );
					RES( 3, getByteAt( *z80->ix + (int8_t)offset ) );
					break;
				case 0xA6:
					printf( "RES 4, (IX+%X)", (int8_t)offset );
					RES( 4, getByteAt( *z80->ix + (int8_t)offset ) );
					break;
				case 0xAE:
					printf( "RES 5, (IX+%X)", (int8_t)offset );
					RES( 5, getByteAt( *z80->ix + (int8_t)offset ) );
					break;
				case 0xB6:
					printf( "RES 6, (IX+%X)", (int8_t)offset );
					RES( 6, getByteAt( *z80->ix + (int8_t)offset ) );
					break;
				case 0xBE:
					printf( "RES 7, (IX+%X)", (int8_t)offset );
					RES( 7, getByteAt( *z80->ix + (int8_t)offset ) );
					break;
				case 0xC6:
					printf( "SET 0, (IX+%X)", (int8_t)offset );
					SET( 0, getByteAt( *z80->ix + (int8_t)offset ) );
					break;
				case 0xCE:
					printf( "SET 1, (IX+%X)", (int8_t)offset );
					SET( 1, getByteAt( *z80->ix + (int8_t)offset ) );
					break;
				case 0xD6:
					printf( "SET 2, (IX+%X)", (int8_t)offset );
					SET( 2, getByteAt( *z80->ix + (int8_t)offset ) );
					break;
				case 0xDE:
					printf( "SET 3, (IX+%X)", (int8_t)offset );
					SET( 3, getByteAt( *z80->ix + (int8_t)offset ) );
					break;
				case 0xE6:
					printf( "SET 4, (IX+%X)", (int8_t)offset );
					SET( 4, getByteAt( *z80->ix + (int8_t)offset ) );
					break;
				case 0xEE:
					printf( "SET 5, (IX+%X)", (int8_t)offset );
					SET( 5, getByteAt( *z80->ix + (int8_t)offset ) );
					break;
				case 0xF6:
					printf( "SET 6, (IX+%X)", (int8_t)offset );
					SET( 6, getByteAt( *z80->ix + (int8_t)offset ) );
					break;
				case 0xFE:
					printf( "SET 7, (IX+%X)", (int8_t)offset );
					SET( 7, getByteAt( *z80->ix + (int8_t)offset ) );
					break;
				default:
					break;
			}
			break;
		case 0xE1:
			printf( "POP IX");
			POP( getWordAt( z80->sp ) ,z80->sp, z80->ix );
			break;
		case 0xE3:
			printf( "EX (SP),IX" );
			EX( getWordAt( z80->sp ), z80->ix );
			break;
		case 0xE5:
			printf( "PUSH IX" );
			PUSH( getWordAt( z80->sp ), z80->sp, z80->ix );
			break;
		case 0xE9:
			//printf( "JP IX" );
			JP( z80->pc, z80->ix );
			break;
		case 0xF9:
			printf( "LD SP,IX" );
			LD16( z80->sp, z80->ix );
			break;
		default:
			break;
		}
}

void execute_multi_byte_opcode_ED( uint8_t *opcode )
{

	switch( *( getNextByte() ) ){
		case 0x42:
			printf( "SBC HL,BC" );
			SBC16( z80->hl, z80->bc, z80->f );
			break;
		case 0x43:
			printf( "LD (%X),BC", readNextWord() );
			LD16( getWordAt( getNextWord() ) ,z80->bc );
			break;
		case 0x46:
			printf( "IM 0" );
			IM0( z80->mode );
			break;
		case 0x47:
			printf( "LD I,A");
			LD( z80->i, z80->a );
			break;
		case 0x4B:
			printf( "LD BC,(%X)", readNextWord() );
			t_counter += 10;
			LD16( z80->bc, getWordAt( getNextWord() ) );
			break;
		case 0x4F:
			printf( "LD R,A");
			LD( z80->r, z80->a );
			break;
		case 0x52:
			printf( "SBC HL,DE");
			t_counter += 15;
			SBC16( z80->hl, z80->de, z80->f );
			break;
		case 0x53:
			printf( "LD (%X),DE", readNextWord() );
			LD16( getWordAt( getNextWord() ) ,z80->de );
			break;
		case 0x56:
			printf( "IM 1" );
			IM1( z80->mode );
			break;
		case 0x57:
			printf( "LD A,I");
			LDAIR( z80->a, z80->i, z80->f, z80->iff2 );
			break;
		case 0x5B:
			printf( "LD DE,(%X)", readNextWord() );
			t_counter += 20;
			LD16( z80->de, getWordAt( getNextWord() ) );
			break;
		case 0x5E:
			printf( "IM 2" );
			IM2( z80->mode );
			break;
		case 0x5F:
			printf( "LD A,R");
			LDAIR( z80->a, z80->r, z80->f, z80->iff2 );
			break;
		case 0x62:
			printf( "SBC HL,HL" );
			SBC16( z80->hl, z80->hl, z80->f );
			break;
		case 0x72:
			printf( "SBC HL,SP" );
			SBC16( z80->hl, z80->sp, z80->f );
			break;
		case 0x73:
			printf( "LD(%X),SP", readNextWord() );
			LD16( getNextWord(), z80->sp );
			break;
		case 0x78:
			printf( "IN A,(C)" );

			IN( z80->a, z80->b, (*portPtrs[254])( READ, *z80->b )/*readPort( port )*/, z80->f );
			break;
		case 0x7B:
			printf( "LD SP,(%X)", readNextWord() );
			LD16( z80->sp, getWordAt( getNextWord() ) );
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
			LDIR( getWordAt( z80->hl ), getWordAt( z80->de ), z80->hl, z80->de, z80->bc, z80->pc, z80->f );
			break;/*
		case 0xB1:
			//printf( "CPIR");
			CPIR();
			break;*/
		case 0xB8:
			printf( "LDDR");
			LDDR( getWordAt( z80->hl ), getWordAt( z80->de ), z80->hl, z80->de, z80->bc, z80->f );
			break;
		case 0xB9:
			//printf( "CPDR");
			//CPDR();
			break;
		default:
			break;
		}
}

static void execute_multi_byte_opcode_FD( uint8_t *opcode )
{
	switch( *(getNextByte()) )
	{
		case 0x21:
			printf( "LD IY,+%X", readNextWord() );
			LD16( z80->iy, getNextWord() );
			break;
		case 0x22:
			printf( "LD (%X),IY", readNextWord() );
			LD16( getWordAt( getNextWord() ) , z80->iy );
			break;
		case 0x23:
			printf( "INC IX" );
			INC16( z80->ix );
			break;
		case 0x2A:
			printf( "LD IY(%X)", readNextWord() );
			LD16( z80->iy, getWordAt( getNextWord() ) );
			break;
		case 0x2B:
			printf( "DEC IY" );
			DEC16( z80->iy );
			break;
		case 0x34:
			printf( "INC IY+%X", readNextByte() );
			INC( getByteAt( *z80->iy + (int8_t)*( getNextByte() ) ), z80->f );
			break;
		case 0x35:
			printf( "DEC IY+%X", readNextByte() );
			DEC( getByteAt( *z80->iy + (int8_t)*( getNextByte() ) ), z80->f );
			break;
		case 0x36:
			printf( "LD (IY+%X),%02X**", (int8_t)readNextByte(), readByteAt( *z80->pc+2 ) );
			LD( getByteAt( *z80->iy + (int8_t)*( getNextByte() ) ), getByteAt( *z80->pc+2 ) );
			getNextByte();
			break;
		case 0x46:
			printf( "LD B,(IY+%X)", (int8_t)readNextByte() );
			LD( z80->b, getByteAt( *z80->iy +(int8_t) *( getNextByte() ) ) );
			break;
		case 0x4E:
			printf( "LD C,(IY+%X)",(int8_t) readNextByte() );
			LD( z80->c, getByteAt( *z80->iy +(int8_t) *( getNextByte() ) ) );
			break;
		case 0x56:
			printf( "LD D,(IY+%X)",(int8_t) readNextByte() );
			LD( z80->d, getByteAt( *z80->iy +(int8_t) *( getNextByte() ) ) );
			break;
		case 0x5E:
			printf( "LD E,(IY+%X)",(int8_t) readNextByte() );
			LD( z80->e, getByteAt( *z80->iy +(int8_t) *( getNextByte() ) ) );
			break;
		case 0x66:
			printf( "LD H,(IY+%X)",(int8_t) readNextByte() );
			LD( z80->h, getByteAt( *z80->iy +(int8_t) *( getNextByte() ) ) );
			break;
		case 0x6E:
			printf( "LD L,(IY+%X)",(int8_t) readNextByte() );
			LD( z80->l, getByteAt( *z80->iy +(int8_t) *( getNextByte() ) ) );
			break;
		case 0x70:
			printf( "LD (IY+%X),B",(int8_t) readNextByte() );
			LD( getByteAt( *z80->iy +(int8_t) *( getNextByte() ) ), z80->b );
			break;
		case 0x71:
			printf( "LD (IY+%X),C",(int8_t) readNextByte() );
			LD( getByteAt( *z80->iy +(int8_t) *( getNextByte() ) ), z80->c );
			break;
		case 0x72:
			printf( "LD (IY+%X),D",(int8_t) readNextByte() );
			LD( getByteAt( *z80->iy +(int8_t) *( getNextByte() ) ), z80->d );
			break;
		case 0x73:
			printf( "LD (IY+%X),E",(int8_t) readNextByte() );
			LD( getByteAt( *z80->iy +(int8_t) *( getNextByte() ) ), z80->e );
			break;
		case 0x74:
			printf( "LD (IY+%X),H",(int8_t) readNextByte() );
			LD( getByteAt( *z80->iy +(int8_t) *( getNextByte() ) ), z80->h );
			break;
		case 0x75:
			printf( "LD (IY+%X),L",(int8_t) readNextByte() );
			LD( getByteAt( *z80->iy +(int8_t) *( getNextByte() ) ), z80->l );
			break;
		case 0x77:
			printf( "LD (IY+%X),A",(int8_t) readNextByte() );
			LD( getByteAt( *z80->iy +(int8_t) *( getNextByte() ) ), z80->a );
			break;
		case 0x7E:
			printf( "LD A,(IY+%X",(int8_t) readNextByte() );
			LD( z80->a, getByteAt( *z80->iy + (int8_t)*( getNextByte() ) ) );
			break;
		case 0x86:
			printf( "ADD (IY+%X)",(int8_t) readNextByte() );
			ADD( z80->a, getByteAt( *z80->iy +(int8_t) *( getNextByte() ) ), z80->f );
			break;
		case 0x8E:
			printf( "ADC (IY+%X)",(int8_t) readNextByte() );
			ADC( z80->a, getByteAt( *z80->iy +(int8_t) *( getNextByte() ) ), z80->f );
			break;
		case 0x96:
			printf( "SUB (IY+%X)",(int8_t) readNextByte() );
			SUB( z80->a, getByteAt( *z80->iy +(int8_t) *( getNextByte() ) ), z80->f );
			break;
		case 0xA6:
			printf( "AND A (IY+%X)",(int8_t) readNextByte() );
			AND( z80->a, getByteAt( *z80->iy +(int8_t) *( getNextByte() ) ), z80->f );
			break;
		case 0xAE:
			printf( "XOR (IY+%X)",(int8_t) readNextByte() );
			XOR( z80->a, getByteAt( *z80->iy +(int8_t) *( getNextByte() ) ) ,z80->f );
			break;
		case 0xB6:
			printf( "OR A,(IY+%X)",(int8_t) readNextByte() );
			OR( z80->a, getByteAt( *z80->iy +(int8_t) *( getNextByte() ) ), z80->f );
			break;
		case 0xBE:
			printf( "CP (IY+%X)",(int8_t) readNextByte() );
			CP( z80->a, getByteAt( *z80->iy +(int8_t) *( getNextByte() ) ) ,z80->f );
			break;
		case 0xCB:
			uint8_t offset = *( getNextByte() );
			switch( *( getNextByte() ) )
			{
				case 0x06:
					printf( "RLC(IY+%X)", offset );
					RLC( getByteAt( *z80->iy + (int8_t) *( getNextByte() ) ), z80->f );
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
					BIT( 0, getByteAt( *z80->iy +(int8_t) offset ), z80->f );
					break;break;
				case 0x4E:
					printf( "BIT 1, (IY+%X)",(int8_t) offset );
					BIT( 1, getByteAt( *z80->iy +(int8_t) offset ), z80->f );
					break;
				case 0x56:
					printf( "BIT 2, (IY+%X)",(int8_t) offset );
					BIT( 2, getByteAt( *z80->iy +(int8_t) offset ), z80->f );
					break;
				case 0x5E:
					printf( "BIT 3, (IY+%X)",(int8_t) offset );
					BIT( 3, getByteAt( *z80->iy +(int8_t) offset ), z80->f );
					break;
				case 0x66:
					printf( "BIT 4, (IY+%X)",(int8_t) offset );
					BIT( 4, getByteAt( *z80->iy +(int8_t) offset ), z80->f );
					break;
				case 0x6E:
					printf( "BIT 5, (IY+%X)",(int8_t) offset );
					BIT( 5, getByteAt( *z80->iy +(int8_t) offset ), z80->f );
					break;
				case 0x76:
					printf( "BIT 6, (IY+%X)",(int8_t) offset );
					BIT( 6, getByteAt( *z80->iy +(int8_t) offset ), z80->f );
					break;
				case 0x7E:
					printf( "BIT 7, (IY+%X)",(int8_t) offset );
					BIT( 7, getByteAt( *z80->iy +(int8_t) offset ), z80->f );
					break;
				case 0x86:
					printf( "RES 0, (IY+%X)",(int8_t) offset );
					RES( 0, getByteAt( *z80->iy +(int8_t) offset ) );
					break;
				case 0x8E:
					printf( "RES 1, (IY+%X)",(int8_t) offset );
					RES( 1, getByteAt( *z80->iy +(int8_t) offset ) );
					break;
				case 0x96:
					printf( "RES 2, (IY+%X)",(int8_t) offset );
					RES( 2, getByteAt( *z80->iy +(int8_t) offset ) );
					break;
				case 0x9E:
					printf( "RES 3, (IY+%X)",(int8_t) offset );
					RES( 3, getByteAt( *z80->iy +(int8_t) offset ) );
					break;
				case 0xA6:
					printf( "RES 4, (IY+%d)",(int8_t) offset );
					RES( 4, getByteAt( *z80->iy +(int8_t) offset ) );
					break;
				case 0xAE:
					printf( "RES 5, (IY+%X)",(int8_t) offset );
					RES( 5, getByteAt( *z80->iy +(int8_t) offset ) );
					break;
				case 0xB6:
					printf( "RES 6, (IY+%X)",(int8_t) offset );
					RES( 6, getByteAt( *z80->iy +(int8_t) offset ) );
					break;
				case 0xBE:
					printf( "RES 7, (IY+%X)",(int8_t) offset );
					RES( 7, getByteAt( *z80->iy +(int8_t) offset ) );
					break;
				case 0xC6:
					printf( "SET 0, (IY+%X)", (int8_t)offset );
					SET( 0, getByteAt( *z80->iy +(int8_t) offset ) );
					break;
				case 0xCE:
					printf( "SET 1, (IY+%X) ",(int8_t) offset );
					SET( 1, getByteAt( *z80->iy + (int8_t)offset ) );
					break;
				case 0xD6:
					printf( "SET 2, (IY+%X) ",(int8_t) offset );
					SET( 2, getByteAt( *z80->iy +(int8_t) offset ) );
					break;
				case 0xDE:
					printf( "SET 3, (IY+%X) ",(int8_t) offset );
					SET( 3, getByteAt( *z80->iy +(int8_t) offset ) );
					break;
				case 0xE6:
					printf( "SET 4, (IY+%X) ",(int8_t) offset );
					SET( 4, getByteAt( *z80->iy +(int8_t) offset ) );
					break;
				case 0xEE:
					printf( "SET 5, (IY+%X) ",(int8_t) offset );
					SET( 5, getByteAt( *z80->iy +(int8_t) offset ) );
					break;
				case 0xF6:
					printf( "SET 6, (IY+%X) ",(int8_t) offset );
					SET( 6, getByteAt( *z80->iy +(int8_t) offset ) );
					break;
				case 0xFE:
					printf( "SET 7, (IY+%X) ",(int8_t) offset );
					SET( 7, getByteAt( *z80->iy +(int8_t) offset ) );
					break;
				default:
					break;
			}
			offset = 0x00;
			break;
		case 0xE1:
			printf( "POP IY");
			POP( getWordAt( z80->sp ), z80->sp, z80->iy );
			break;
		case 0xE3:
			printf( "EX (SP),IY");
			EX( getWordAt( z80->sp ), z80->iy );
			break;
		case 0xE5:
			printf( "PUSH IY" );
			PUSH( getWordAt( z80->sp ), z80->sp, z80->iy );
			break;
		case 0xE9:
			printf( "JP(IY)" );
			JP( z80->pc, getWordAt( z80->iy ) );
			break;
		case 0xF9:
			printf( "LD SP,IY");
			LD16( z80->sp, z80->iy );
			break;
		case 0xFE:
			printf( "CP %X", readNextByte() );
			CP( z80->a, getNextByte(), z80->f );
			break;
		case 0xFF:
			printf( "RST 38H");
			RST( 0x38, getWordAt( z80->sp ), z80->sp, z80->pc );
			break;
		default:
			break;
	}
}


// Return the 16-bit value of two 8-bit combined z80isters
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
	return getByte( ++*z80->pc );
}

// Read the next byte along, does not progress pc
uint8_t readNextByte(){
	// Dereference into the memory location and return the value, progress pc by 1
	return *( getByte( *z80->pc+1 ) );
}

uint16_t* getNextWord(){
	// Dereference into the memory location and return the word of the next two bytes
	uint16_t* word = getWord( ++*z80->pc );
	// Increment pc again to point at at the second byte in the word, ready for next increment by execution loop
	++*z80->pc;
	return word;
}

// read next word along, does not progress pc
uint16_t readNextWord(){
	// Dereference into the memory location and return the word of the next to bytes
	return *( getWord( *z80->pc+1 ) );
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

// ** Used internally to read in data from ports and if needed the address bus **/
// Read the port at the given 8-bit address on the lower-half of the address bus.
// Note this param is a WORD as the address the Bus is still 16-bits wide.

uint8_t readPort(uint16_t portAddrs){

	// If called from IN r,(C) then only need to be concerned with lower-half port address 0x00FE
	if( portAddrs == 0x00FE ){
		return 0x1F;
	}

	// If keyboard ports
	switch( portAddrs ){
		case 0xFEFE:
			//return readKeyboard( 1 );
			return 0x8F;
			break;
		case 0xFDFE:
			return 0xBF;
			break;
		case 0xFBFE:
			return 0xBF;
			break;
		case 0xF7FE:
			return 0xBF;
			break;
		case 0xEFFE:
			return 0xBF;
			break;
		case 0xDFFE:
			return 0xBF;
			break;
		case 0xBFFE:
			return 0xBF;
			break;
		case 0x7FFE:
			return 0xBF;
			break;
		default:
			break;
	}

	return 0;
}

z80_instruction singleByteInstructionLookup[] = {
	{"NOP", 2, NOP }                       /*0x00*/
    /*"LD BC,+%X",                 /*0x01*/
    /*"LD(BC),A",                  /*0x02*/
    /*"INC BC",                    /*0x03*/
    /*"INC B",                     /*0x04*/
    /*"DEC B",                     /*0x05*/
    /*"LD B,+%X",                  /*0x06*/
    /*"RLCA",                      /*0x07*/
    /*"EX AF,AF'",                 /*0x08*/
    /*"ADD HL,BC",                 /*0x09*/
    /*"LD A,(BC)",                 /*0x0A*/
    /*"DEC BC",                    /*0x0B*/
    /*"INC C",                     /*0x0C*/
    /*"DEC C",                     /*0x0D*/
    /*"LD C,+%X",                  /*0x0E*/
    /*"RRCA",                      /*0x0F*/
    /*"DJNZ %d",                   /*0x10*/
    /*"LD DE,+%X",                 /*0x11*/
    /*"LD(DE),A",                  /*0x12*/
    /*"INC DE",                    /*0x13*/
    /*"INC D",                     /*0x14*/
    /*"DEC D",                     /*0x15*/
    /*"LD D,+%X",                  /*0x16*/
    /*"RLA",                       /*0x17*/
    /*"JR %X",                     /*0x18*/
    /*"ADD HL,DE",                 /*0x19*/
    /*"LD A,(DE)",                 /*0x1A*/
    /*"DEC DE",                    /*0x1B*/
    /*"INC E",                     /*0x1C*/
    /*"DEC E",                     /*0x1D*/
    /*"LD E,+%X",                  /*0x1E*/
    /*"RRA",                       /*0x1F*/
    /*"JR NZ,%d",                  /*0x20*/
    /*"LD HL,+%X",                 /*0x21*/
    /*"LD (%X),HL",                /*0x22*/
    /*"INC HL",                    /*0x23*/
    /*"INC H",                     /*0x24*/
    /*"DEC H",                     /*0x25*/
    /*"LD H,+%X",                  /*0x26*/
    /*"DAA",                       /*0x27*/
    /*"JR Z,+%X",                  /*0x28*/
    /*"ADD HL,HL",                 /*0x29*/
    /*"LD HL,(%X)",                /*0x2A*/
    /*"DEC HL",                    /*0x2B*/
    /*"INC L",                     /*0x2C*/
    /*"DEC L",                     /*0x2D*/
    /*"LD L,+%X",                  /*0x2E*/
    /*"CPL",                       /*0x2F*/
    /*"JR NC,%d",                  /*0x30*/
    /*"LD SP,+%X",                 /*0x31*/
    /*"LD (%X),A",                 /*0x32*/
    /*"INC SP",                    /*0x33*/
    /*"INC (HL)",                  /*0x34*/
    /*"DEC (HL)",                  /*0x35*/
    /*"LD (HL),+%X",               /*0x36*/
    /*"SCF",                       /*0x37*/
    /*"JR C,%d",                   /*0x38*/
    /*"ADD HL,SP",                 /*0x39*/
    /*"LD A,(%X)",                 /*0x3A*/
    /*"DEC SP",                    /*0x3B*/
    /*"INC A",                     /*0x3C*/
    /*"DEC A",                     /*0x3D*/
    /*"LD A,+%X",                  /*0x3E*/
    /*"CCF",                       /*0x3F*/
    /*"LD B,B",                    /*0x40*/
    /*"LD B,C",                    /*0x41*/
    /*"LD B,D",                    /*0x42*/
    /*"LD B,E",                    /*0x43*/
    /*"LD B,H",                    /*0x44*/
    /*"LD B,L",                    /*0x45*/
    /*"LD B,(HL)",                 /*0x46*/
    /*"LD B,A",                    /*0x47*/
    /*"LD C,B",                    /*0x48*/
    /*"LD C,C",                    /*0x49*/
    /*"LD C,D",                    /*0x4A*/
    /*"LD C,E",                    /*0x4B*/
    /*"LD C,H",                    /*0x4C*/
    /*"LD C,L",                    /*0x4D*/
    /*"LD C,(HL)",                 /*0x4E*/
    /*"LD C,A",                    /*0x4F*/
    /*"LD D,B",                    /*0x50*/
    /*"LD D,C",                    /*0x51*/
    /*"LD D,D",                    /*0x52*/
    /*"LD D,E",                    /*0x53*/
    /*"LD D,H",                    /*0x54*/
    /*"LD D,L",                    /*0x55*/
    /*"LD D,(HL)",                 /*0x56*/
    /*"LD D,A",                    /*0x57*/
    /*"LD E,B",                    /*0x58*/
    /*"LD E,C",                    /*0x59*/
    /*"LD E,D",                    /*0x5A*/
    /*"LD E,E",                    /*0x5B*/
    /*"LD E,H",                    /*0x5C*/
    /*"LD E,L",                    /*0x5D*/
    /*"LD E,(HL)",                 /*0x5E*/
    /*"LD E,A",                    /*0x5F*/
    /*"LD H,B",                    /*0x60*/
    /*"LD H,C",                    /*0x61*/
    /*"LD H,D",                    /*0x62*/
    /*"LD H,E",                    /*0x63*/
    /*"LD H,H",                    /*0x64*/
    /*"LD H,L",                    /*0x65*/
    /*"LD H,(HL)",                 /*0x66*/
    /*"LD H,A",                    /*0x67*/
    /*"LD L,B",                    /*0x68*/
    /*"LD L,C",                    /*0x69*/
    /*"LD L,D",                    /*0x6A*/
    /*"LD L,E",                    /*0x6B*/
    /*"LD L,H",                    /*0x6C*/
    /*"LD L,L",                    /*0x6D*/
    /*"LD L,(HL)",                 /*0x6E*/
    /*"LD L,A",                    /*0x6F*/
    /*"LD (HL),B",                 /*0x70*/
    /*"LD (HL),C",                 /*0x71*/
    /*"LD (HL),D",                 /*0x72*/
    /*"LD (HL),E",                 /*0x73*/
    /*"LD (HL),H",                 /*0x74*/
    /*"LD (HL),L",                 /*0x75*/
    /*"HALT",                      /*0x76*/
    /*"LD (HL),A",                 /*0x77*/
    /*"LD A,B",                    /*0x78*/
    /*"LD A,C",                    /*0x79*/
    /*"LD A,D",                    /*0x7A*/
    /*"LD A,E",                    /*0x7B*/
    /*"LD A,H",                    /*0x7C*/
    /*"LD A,L",                    /*0x7D*/
    /*"LD A,(HL)",                 /*0x7E*/
    /*"LD A,A",                    /*0x7F*/
    /*"ADD A,B",                   /*0x80*/
    /*"ADD A,C",                   /*0x81*/
    /*"ADD A,D",                   /*0x82*/
    /*"ADD A,E",                   /*0x83*/
    /*"ADD A,H",                   /*0x84*/
    /*"ADD A,L",                   /*0x85*/
    /*"ADD (HL)",                  /*0x86*/
    /*"ADD A,A",                   /*0x87*/
    /*"ADC A,B",                   /*0x88*/
    /*"ADC A,C",                   /*0x89*/
    /*"ADC A,D",                   /*0x8A*/
    /*"ADC A,E",                   /*0x8B*/
    /*"ADC A,H",                   /*0x8C*/
    /*"ADC A,L",                   /*0x8D*/
    /*"ADC A,(HL)",                /*0x8E*/
    /*"ADC A,A",                   /*0x8F*/
    /*"SUB B",                     /*0x90*/
    /*"SUB C",                     /*0x91*/
    /*"SUB D",                     /*0x92*/
    /*"SUB E",                     /*0x93*/
    /*"SUB H",                     /*0x94*/
    /*"SUB L",                     /*0x95*/
    /*"SUB (HL)",                  /*0x96*/
    /*"SUB A",                     /*0x97*/
    /*"SBC A,B",                   /*0x98*/
    /*"SBC A,C",                   /*0x99*/
    /*"SBC A,D",                   /*0x9A*/
    /*"SBC A,E",                   /*0x9B*/
    /*"SBC A,H",                   /*0x9C*/
    /*"SBC A,L",                   /*0x9D*/
    /*"SBC A,(HL)",                /*0x9E*/
    /*"SBC A,A",                   /*0x9F*/
    /*"AND B",                     /*0xA0*/
    /*"AND C",                     /*0xA1*/
    /*"AND D",                     /*0xA2*/
    /*"AND E",                     /*0xA3*/
    /*"AND H",                     /*0xA4*/
    /*"AND L",                     /*0xA5*/
    /*"AND (HL)",                  /*0xA6*/
    /*"AND A",                     /*0xA7*/
    /*"XOR B",                     /*0xA8*/
    /*"XOR C",                     /*0xA9*/
    /*"XOR D",                     /*0xAA*/
    /*"XOR E",                     /*0xAB*/
    /*"XOR H",                     /*0xAC*/
    /*"XOR L",                     /*0xAD*/
    /*"XOR (HL)",                  /*0xAE*/
    /*"XOR A",                     /*0xAF*/
    /*"OR B",                      /*0xB0*/
    /*"OR C",                      /*0xB1*/
    /*"OR D",                      /*0xB2*/
    /*"OR E",                      /*0xB3*/
    /*"OR H",                      /*0xB4*/
    /*"OR L",                      /*0xB5*/
    /*"OR(HL)",                    /*0xB6*/
    /*"OR A",                      /*0xB7*/
    /*"CP B",                      /*0xB8*/
    /*"CP C",                      /*0xB9*/
    /*"CP D",                      /*0xBA*/
    /*"CP E",                      /*0xBB*/
    /*"CP H",                      /*0xBC*/
    /*"CP L",                      /*0xBD*/
    /*"CP (HL)",                   /*0xBE*/
    /*"CP A",                      /*0xBF*/
    /*"RET NZ",                    /*0xC0*/
    /*"POP BC",                    /*0xC1*/
    /*"JP NZ, %X",                 /*0xC2*/
    /*"JP %X",                     /*0xC3*/
    /*"CALLNZ %X",                 /*0xC4*/
    /*"PUSH BC",                   /*0xC5*/
    /*"ADD A,%X",                  /*0xC6*/
    /*"RST 00H",                   /*0xC7*/
    /*"RET Z",                     /*0xC8*/
    /*"RET",                       /*0xC9*/
    /*"JP Z,%X",                   /*0xCA*/
    /*"CALL Z,%X",                 /*0xCB*/
    /*"CALL %X",                   /*0xCC*/
    /*"ADC A,%X",                  /*0xCD*/
    /*"RST 08H",                   /*0xCE*/
    /*"RET NC",                    /*0xCF*/
    /*"POP DE",                    /*0xD0*/
    /*"JP NC,%X",                  /*0xD1*/
    /*"OUT (+%X),A",               /*0xD2*/
    /*"CALL NC,%X",                /*0xD3*/
    /*"PUSH DE",                   /*0xD4*/
    /*"SUB %X",                    /*0xD5*/
    /*"RST 10",                    /*0xD6*/
    /*"RET C",                     /*0xD7*/
    /*"EXX",                       /*0xD8*/
    /*"JP C,%X",                   /*0xD9*/
    /*"IN A,(C)",                  /*0xDA*/
    /*"CALL C,%X",                 /*0xDB*/
    /*"SBC A,%X",                  /*0xDC*/
    /*"RST 18H",                   /*0xDD*/
    /*"RET PO,%X",                 /*0xDE*/
    /*"POP HL",                    /*0xDF*/
    /*"JP PO,%X ",                 /*0xE0*/
    /*"EX (SP),HL",                /*0xE1*/
    /*"CALL PO,%X",                /*0xE2*/
    /*"PUSH HL",                   /*0xE3*/
    /*"AND %X",                    /*0xE4*/
    /*"RST 20H",                   /*0xE5*/
    /*"RET PE,%X",                 /*0xE6*/
    /*"JP (HL)",                   /*0xE7*/
    /*"JP PE,%X",                  /*0xE8*/
    /*"EX DE,HL",                  /*0xE9*/
    /*"CALL PE,%X",                /*0xEA*/
    /*"XOR %X",                    /*0xEB*/
    /*"RST 28H",                   /*0xEC*/
    /*"RET P",                     /*0xED*/
    /*"POP AF",                    /*0xEE*/
    /*"JP P,%X",                   /*0xEF*/
    /*"DI",                        /*0xF0*/
    /*"CALL P,%X",                 /*0xF1*/
    /*"PUSH AF",                   /*0xF2*/
    /*"OR %X",                     /*0xF3*/
    /*"RST 30H",                   /*0xF4*/
    /*"RET M,%X",                  /*0xF5*/
    /*"LD SP,HL",                  /*0xF6*/
    /*"JP M,%X",                   /*0xF7*/
    /*"EI",                        /*0xF8*/
    /*"CALL M,%X",                 /*0xF9*/
};                               /*0xFA*/
                                 /*0xFB*/
                                  /*0xFC*/
                                  /*0xFD*/
                                  /*0xFE*/
                                  /*0xFF*/

