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
//#include "../include/ula.h"
#include "../include/memory.h"
#include "../include/disassembler.h"

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

// 8-Bit Z80 main general purpose registers
union _AF{
	uint8_t _af[2];		// Accumulater	// Flags register (Set = 1, Reset = 0)
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
	uint8_t _af[2];		// Accumulater	// Flags register (Set = 1, Reset = 0)
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


// 8-Bit special registers - Interrupt vector
union _IR{
	uint8_t _ir[2];
	uint16_t ir;
}ir;


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
	//printf("0x%04X\t%02X\t", *reg->pc, totalMem[*reg->pc] );
	
	++*reg->r;

	if( *opcode == 0xCB || *opcode == 0xDD || *opcode == 0xED || *opcode == 0xFD)
	{

		//disassemble_multi_byte_opcode( opcode );
		execute_multi_byte_opcode( opcode );

	}
	else if ( *opcode >= 0x00 && *opcode <= 0xFF)
	{
		//disassemble_single_byte_opcode( opcode );
		execute_single_byte_opcode( opcode );
	}

	assert( *opcode >= 0x00 && *opcode <= 0xFF );
}

void execute_single_byte_opcode( uint8_t *opcode )
{
	switch( *opcode )
	{
	
	//	case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x47: case 0x48: case 0x49: case 0x4A: case 0x4B: case 0x4C: case 0x4D: case 0x4F:
	//	case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x57: case 0x58: case 0x59: case 0x5A: case 0x5B: case 0x5C: case 0x5D: case 0x5F: 
	//	case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x67: case 0x68: case 0x69: case 0x6A: case 0x6B: case 0x6C: case 0x6D: case 0x6F: 
	//	case 0x78: case 0x79: case 0x7A: case 0x7B: case 0x7C: case 0x7D: case 0x7F:
		
		//break;
		case 0x00:
			NOP();
			break;
		case 0x01:
			LD16( reg->bc, getNextWord() );
			break;
		case 0x02:
			LD( getByteAt( *reg->bc ) ,reg->a );
			break;
		case 0x03:
			INC16( reg->bc );
			break;
		case 0x04:
			INC( reg->b, reg->f );
			break;
		case 0x05:
			DEC( reg->b, reg->f );
			break;
		case 0x06:
			LD( reg->b, getNextByte() );
			break;
		case 0x07:
			RLCA( reg->a, reg->f );
			break;
		case 0x08:
			EX( reg->af, reg->altaf );
			break;
		case 0x09:
			ADD16( reg->hl, reg->bc, reg->f );
			break;
		case 0x0A:
			t_counter += 7;
			LD( reg->a, getByteAt( *reg->bc ) );
			break;
		case 0x0B:
			DEC16( reg->bc );
			break;
		case 0x0C:
			INC( reg->c, reg->f );
			break;
		case 0x0D:
			DEC( reg->c, reg->f );
			break;
		case 0x0E:
			LD( reg->c, getNextByte() );
			break;
		case 0x0F:
			RRCA( reg->a, reg->f );
			break;
		case 0x10:
			DJNZ( getNextByte(), reg->b, reg->pc );
			break;
		case 0x11:
			LD16( reg->de, getNextWord() );
			break;
		case 0x12:
			LD( getByteAt( *reg->de ) ,reg->a );
			break;
		case 0x13:
			INC16( reg->de );
			break;
		case 0x14:
			INC( reg->d, reg->f );
			break;
		case 0x15:
			DEC( reg->d, reg->f );
			break;
		case 0x16:
			LD( reg->d, getNextByte() );
			break;
		case 0x17:
			RLA( reg->a, reg->f );
			break;
		case 0x18:
			JR( getNextByte(), reg->pc );
			break;
		case 0x19:
			t_counter += 11;
			ADD16( reg->hl, reg->de, reg->f );
			break;
		case 0x1A:
			LD( reg->a, getByteAt( *reg->de ) );
			break;
		case 0x1B:
			DEC16( reg->de );
			break;
		case 0x1C:
			INC( reg->e, reg->f );
			break;
		case 0x1D:
			DEC( reg->e, reg->f );
			break;
		case 0x1E:
			LD( reg->e, getNextByte() );
			break;
		case 0x1F:
			RRA( reg->a, reg->f );
			break;
		case 0x20:
			t_counter += 7;
			JRNZ( reg->pc, getNextByte(), reg->f );
			break;
		case 0x21:
			t_counter += 16;
			LD16( reg->hl, getNextWord() );
			break;
		case 0x22:
			LD16( getWordAt( getNextWord() ), reg->hl );
			break;
		case 0x23:
			t_counter += 6;
			INC16( reg->hl );
			break;
		case 0x24:
			INC( reg->h, reg->f );
			break;
		case 0x25:
			DEC( reg->h, reg->f );
			break;
		case 0x26:
			LD( reg->h, getNextByte() );
			break;/*
		case 0x27:
			break;*/
		case 0x28:
			t_counter += 12;
			JRZ( reg->pc, getNextByte(), reg->f );
			break;
		case 0x29:
			ADD16( reg->hl, reg->hl, reg->f );
			break;
		case 0x2A:
			LD16( reg->hl, getWordAt( getNextWord() ) );
			break;
		case 0x2B:
			t_counter += 6;
			DEC16( reg->hl );
			break;
		case 0x2C:
			INC( reg->l, reg->f );
			break;
		case 0x2D:
			DEC( reg->l, reg->f );
			break;
		case 0x2E:
			LD( reg->l, getNextByte() );
			break;
		case 0x2F:
			CPL( reg->a, reg->f );
			break;
		case 0x30:
			t_counter += 12;
			JRNC( reg->pc, getNextByte(), reg->f );
			break;
		case 0x31:
			LD16( reg->hl, getNextWord() );
			break;
		case 0x32:
			LD( getByteAt( *( getNextWord() ) ) ,reg->a );
			break;
		case 0x33:
			INC16( reg->sp );
			break;
		case 0x34:
			INC( getByteAt( *reg->hl ), reg->f );
			break;
		case 0x35:
			t_counter += 6;
			DEC( getByteAt( *reg->hl ), reg->f );
			break;
		case 0x36:
			t_counter += 7;
			LD( getByteAt( *reg->hl ), getNextByte() );
			break;
		case 0x37:
			SCF( reg->f );
			break;
		case 0x38:
			JRC( reg->pc, getNextByte(), reg->f );
			break;
		case 0x39:
			ADD16( reg->hl, reg->sp, reg->f );
			break;
		case 0x3A:
			LD( reg->a, getByteAt( *(getNextWord()) ) );
			break;
		case 0x3B:
			DEC16( reg->sp );
			break;
		case 0x3C:
			INC( reg->a, reg->f );
			break;
		case 0x3D:
			DEC( reg->a, reg->f );
			break;
		case 0x3E:
			LD( reg->a, getNextByte() );
			t_counter += 13;
			break;
		case 0x3F:
			CCF( reg->f );
			break;
		case 0x40:
			t_counter += 4;
			LD( reg->b, reg->b );
			break;
		case 0x41:
			t_counter += 4;
			LD( reg->b, reg->c );
			break;
		case 0x42:
			t_counter += 4;
			LD( reg->b, reg->d );
			break;
		case 0x43:
			t_counter += 4;
			LD( reg->b, reg->e );
			break;
		case 0x44:
			t_counter += 4;
			LD( reg->b, reg->h );
			break;
		case 0x45:
			t_counter += 4;
			LD( reg->b, reg->l );
			break;
		case 0x46:
			t_counter += 4;
			LD( reg->b, getByteAt( *reg->hl ) );
			break;
		case 0x47:
			t_counter += 4;
			LD( reg->b, reg->a );
			break;
		case 0x48:
			t_counter += 4;
			LD( reg->c, reg->b );
			break;
		case 0x49:
			t_counter += 4;
			LD( reg->c, reg->c );
			break;
		case 0x4A:
			t_counter += 4;
			LD( reg->c, reg->d );
			break;
		case 0x4B:
			t_counter += 4;
			LD( reg->c, reg->e );
			break;
		case 0x4C:
			t_counter += 4;
			LD( reg->c, reg->h );
			break;
		case 0x4D:
			t_counter += 4;
			LD( reg->c, reg->l );
			break;
		case 0x4E:
			t_counter += 7;
			LD( reg->c, getByteAt( *reg->hl ) );
			break;
		case 0x4F:
			t_counter += 4;
			LD( reg->c, reg->a );
			break;
		case 0x50:
			t_counter += 4;
			LD( reg->d, reg->b );
			break;
		case 0x51:
			t_counter += 4;
			LD( reg->d, reg->c );
			break;
		case 0x52:
			t_counter += 4;
			LD( reg->d, reg->d );
			break;
		case 0x53:
			t_counter += 4;
			LD( reg->d, reg->e );
			break;
		case 0x54:
			t_counter += 4;
			LD( reg->d, reg->h );
			break;
		case 0x55:
			t_counter += 4;
			LD( reg->d, reg->l );
			break;
		case 0x56:
			t_counter += 7;
			LD( reg->d, getByteAt( *reg->hl ) );
			break;
		case 0x57:
			t_counter += 4;
			LD( reg->d, reg->a );
			break;
		case 0x58:
			t_counter += 4;
			LD( reg->e, reg->b );
			break;
		case 0x59:
			t_counter += 4;
			LD( reg->e, reg->c );
			break;
		case 0x5A:
			t_counter += 4;
			LD( reg->e, reg->d );
			break;
		case 0x5B:
			t_counter += 4;
			LD( reg->e, reg->e );
			break;
		case 0x5C:
			t_counter += 4;
			LD( reg->e, reg->h );
			break;
		case 0x5D:
			t_counter += 4;
			LD( reg->e, reg->l );
			break;
		case 0x5E:
			t_counter += 7;
			LD( reg->e, getByteAt( *reg->hl ) );
			break;
		case 0x5F:
			t_counter += 4;
			LD( reg->e, reg->a );
			break;
		case 0x60:
			t_counter += 4;
			LD( reg->h, reg->b );
			break;
		case 0x61:
			t_counter += 4;
			LD( reg->h, reg->c );
			break;
		case 0x62:
			t_counter += 4;
			LD( reg->h, reg->d );
			break;
		case 0x63:
			t_counter += 4;
			LD( reg->h, reg->e );
			break;
		case 0x64:
			t_counter += 4;
			LD( reg->h, reg->h );
			break;
		case 0x65:
			t_counter += 4;
			LD( reg->h, reg->l );
			break;
		case 0x66:
			LD( reg->h, getByteAt( *reg->hl ) );
			t_counter += 7;
			break;
		case 0x67:
			t_counter += 4;
			LD( reg->h, reg->a );
			break;
		case 0x68:
			t_counter += 4;
			LD( reg->l, reg->b );
			break;
		case 0x69:
			t_counter += 4;
			LD( reg->l, reg->c );
			break;
		case 0x6A:
			t_counter += 4;
			LD( reg->l, reg->d );
			break;
		case 0x6B:
			t_counter += 4;
			LD( reg->l, reg->e );
			break;
		case 0x6C:
			t_counter += 4;
			LD( reg->l, reg->h );
			break;
		case 0x6D:
			t_counter += 4;
			LD( reg->l, reg->l );
			break;
		case 0x6E:
			LD( reg->l, getByteAt( *reg->hl ) );
			t_counter += 7;
			break;
		case 0x6F:
			t_counter += 4;
			LD( reg->l, reg->a );
			break;
		case 0x70:
			t_counter += 7;
			LD( getByteAt( *reg->hl ) ,reg->b );
			break;
		case 0x71:
			t_counter += 7;
			LD( getByteAt( *reg->hl ), reg->c );
			break;
		case 0x72:
			t_counter += 7;
			LD( getByteAt( *reg->hl ), reg->d );
			break;
		case 0x73:
			t_counter += 7;
			LD( getByteAt( *reg->hl ), reg->e );
			break;
		case 0x74:
			t_counter += 7;
			LD( getByteAt( *reg->hl ), reg->h );
			break;
		case 0x75:
			t_counter += 7;
			LD( getByteAt( *reg->hl ), reg->l );
			break;/*
		case 0x76:
			HALT
			break;*/
		case 0x77:
			t_counter += 7;
			LD( getByteAt( *reg->hl ) ,reg->a );
			break;
		case 0x78:
			t_counter += 4;
			LD( reg->a, reg->b );
			break;
		case 0x79:
			t_counter += 4;
			LD( reg->a, reg->c );
			break;
		case 0x7A:
			t_counter += 4;
			LD( reg->a, reg->d );
			break;
		case 0x7B:
			LD( reg->a, reg->e );
			t_counter += 4;
			break;
		case 0x7C:
			LD( reg->a, reg->h );
			t_counter += 4;
			break;
		case 0x7D:
			LD( reg->a, reg->l );
			t_counter += 4;
			break;
		case 0x7E:
			t_counter += 7;
			LD( reg->a, getByteAt( *reg->hl ) );
			break;
		case 0x7F:
			LD( reg->a, reg->a );
			t_counter += 4;
			break;
		case 0x80:
			ADD( reg->a, reg->b, reg->f );
			t_counter += 4;
			break;
		case 0x81:
			ADD( reg->a, reg->c, reg->f );
			t_counter += 4;
			break;
		case 0x82:
			ADD( reg->a, reg->d, reg->f );
			t_counter += 4;
			break;
		case 0x83:
			t_counter += 4;
			ADD( reg->a, reg->e, reg->f );
			break;
		case 0x84:
			t_counter += 4;
			ADD( reg->a, reg->h, reg->f );
			break;
		case 0x85:
			t_counter += 4;
			ADD( reg->a, reg->l, reg->f );
			break;
		case 0x86:
			ADD( reg->a, getByteAt( *reg->hl ), reg->f );
			t_counter += 4;
			break;
		case 0x87:
			ADD( reg->a, reg->a, reg->f );
			t_counter += 4;
			break;
		case 0x88:
			ADC( reg->a, reg->b, reg->f );
			t_counter += 4;
			break;
		case 0x89:
			ADC( reg->a, reg->c, reg->f );
			t_counter += 4;
			break;
		case 0x8A:
			ADC( reg->a, reg->d, reg->f );
			t_counter += 4;
			break;
		case 0x8B:
			ADC( reg->a, reg->e, reg->f );
			t_counter += 4;
			break;
		case 0x8C:
			ADC( reg->a, reg->h, reg->f );
			t_counter += 4;
			break;
		case 0x8D:
			ADC( reg->a, reg->l, reg->f );
			t_counter += 4;
			break;
		case 0x8E:
			ADC( reg->a, getByteAt( *reg->hl ), reg->f );
			break;
		case 0x8F:
			ADC( reg->a, reg->a, reg->f );
			t_counter += 4;
			break;
		case 0x90:
			SUB( reg->a, reg->b, reg->f );
			t_counter += 4;
			break;
		case 0x91:
			SUB( reg->a, reg->c, reg->f );
			t_counter += 4;
			break;
		case 0x92:
			SUB( reg->a, reg->d, reg->f );
			t_counter += 4;
			break;
		case 0x93:
			SUB( reg->a, reg->e, reg->f );
			t_counter += 4;
			break;
		case 0x94:
			SUB( reg->a, reg->h, reg->f );
			t_counter += 4;
			break;
		case 0x95:
			SUB( reg->a, reg->l, reg->f );
			t_counter += 4;
			break;
		case 0x96:
			SUB( reg->a, getByteAt( *reg->hl ), reg->f );
			break;
		case 0x97:
			SUB( reg->a, reg->a, reg->f );
			t_counter += 4;
			break;
		case 0x98:
			SBC( reg->a, reg->b, reg->f );
			t_counter += 4;
			break;
		case 0x99:
			SBC( reg->a, reg->c, reg->f );
			t_counter += 4;
			break;
		case 0x9A:
			SBC( reg->a, reg->d, reg->f );
			t_counter += 4;
			break;
		case 0x9B:
			SBC( reg->a, reg->e, reg->f );
			t_counter += 4;
			break;
		case 0x9C:
			SBC( reg->a, reg->h, reg->f );
			t_counter += 4;
			break;
		case 0x9D:
			SBC( reg->a, reg->l, reg->f );
			t_counter += 4;
			break;
		case 0x9E:
			SBC( reg->a, getByteAt( *reg->hl ), reg->f );
			break;
		case 0x9F:
			SBC( reg->a, reg->a, reg->f );
			t_counter += 4;
			break;
		case 0xA0:
			AND( reg->a, reg->b, reg->f );
			t_counter += 4;
			break;
		case 0xA1:
			AND( reg->a, reg->c, reg->f );
			t_counter += 4;
			break;
		case 0xA2:
			AND( reg->a, reg->d, reg->f );
			t_counter += 4;
			break;
		case 0xA3:
			AND( reg->a, reg->e, reg->f );
			t_counter += 4;
			break;
		case 0xA4:
			AND( reg->a, reg->h, reg->f );
			t_counter += 4;
			break;
		case 0xA5:
			AND( reg->a, reg->l, reg->f );
			t_counter += 4;
			break;
		case 0xA6:
			AND( reg->a, getByteAt( *reg->hl ), reg->f );
			break;
		case 0xA7:
			t_counter += 4;
			AND( reg->a, reg->a, reg->f );
			break;
		case 0xA8:
			XOR( reg->a, reg->b, reg->f );
			t_counter += 4;
			break;
		case 0xA9:
			XOR( reg->a, reg->c, reg->f );
			t_counter += 4;
			break;
		case 0xAA:
			XOR( reg->a, reg->d, reg->f );
			t_counter += 4;
			break;
		case 0xAB:
			XOR( reg->a, reg->e, reg->f );
			t_counter += 4;
			break;
		case 0xAC:
			XOR( reg->a, reg->h, reg->f );
			t_counter += 4;
			break;
		case 0xAD:
			XOR( reg->a, reg->l, reg->f );
			t_counter += 4;
			break;
		case 0xAE:
			XOR( reg->a, getByteAt( *reg->hl ), reg->f );
			break;
		case 0xAF:
			XOR( reg->a, reg->a, reg->f );
			t_counter += 4;
			break;
		case 0xB0:
			OR( reg->a, reg->b, reg->f );
			t_counter += 4;
			break;
		case 0xB1:
			OR( reg->a, reg->c, reg->f );
			t_counter += 4;
			break;
		case 0xB2:
			OR( reg->a, reg->d, reg->f );
			t_counter += 4;
			break;
		case 0xB3:
			OR( reg->a, reg->e, reg->f );
			t_counter += 4;
			break;
		case 0xB4:
			OR( reg->a, reg->h, reg->f );
			t_counter += 4;
			break;
		case 0xB5:
			OR( reg->a, reg->l, reg->f );
			t_counter += 4;
			break;
		case 0xB6:
			OR( reg->a, getByteAt( *reg->hl ), reg->f );
			break;
		case 0xB7:
			t_counter += 4;
			OR( reg->a, reg->a, reg->f );
			break;
		case 0xB8:
			t_counter += 4;
			CP( reg->a, reg->b, reg->f );
			break;
		case 0xB9:
			CP( reg->a, reg->c, reg->f );
			t_counter += 4;
			break;
		case 0xBA:
			t_counter += 4;
			CP( reg->a, reg->d, reg->f );
			break;
		case 0xBB:
			t_counter += 4;
			CP( reg->a, reg->e, reg->f );
			break;
		case 0xBC:
			t_counter += 4;
			CP( reg->a, reg->h, reg->f );
			break;
		case 0xBD:
			t_counter += 4;
			CP( reg->a, reg->l, reg->f );
			break;
		case 0xBE:
			CP( reg->a, getByteAt( *reg->hl ), reg->f );
			break;
		case 0xBF:
			t_counter += 4;
			CP( reg->a, reg->a, reg->f );
			break;
		case 0xC0:
			t_counter += 11;
			RETNZ( reg->pc, getWordAt( reg->sp ), reg->sp, reg->f );
			break;
		case 0xC1:
			t_counter += 10;
			POP( getWordAt( reg->sp ) ,reg->sp, reg->bc );
			break;
		case 0xC2:
			JPNZ( reg->pc, getNextWord(), reg->f );
			break;
		case 0xC3:
			JP( reg->pc, getNextWord() );
			break;
		case 0xC4:
			CALLNZ( getNextWord(), getWordAt( reg->sp ), reg->sp, reg->pc, reg->f );
			break;
		case 0xC5:
			t_counter += 11;
			PUSH( getWordAt( reg->sp ), reg->sp, reg->bc );
			break;
		case 0xC6:
			ADD( reg->a, getNextByte(), reg->f );
			break;
		case 0xC7:
			RST( 0x00, getWordAt( reg->sp ), reg->sp, reg->pc );
			break;
		case 0xC8:
			t_counter += 11;
			RETZ( reg->pc, getWordAt( reg->sp ), reg->sp, reg->f );
			break;
		case 0xC9:
			t_counter += 10;
			RET( reg->pc, getWordAt( reg->sp ), reg->sp );
			break;
		case 0xCA:
			JPZ( reg->pc, getNextWord(), reg->f );
			break;
		case 0xCB:
			execute_multi_byte_opcode(opcode);
			break;
		case 0xCC:
			CALLZ( getNextWord(), getWordAt( reg->sp ), reg->sp, reg->pc, reg->f );
			break;
		case 0xCD:
			CALL( getNextWord(), getWordAt( reg->sp ), reg->sp, reg->pc );
			break;
		case 0xCE:
			ADC( reg->a, getNextByte(), reg->f );
			break;
		case 0xCF:
			RST( 0x08, getWordAt( reg->sp ), reg->sp, reg->pc );
			break;
		case 0xD0:
			RETNC( reg->pc, getWordAt( reg->sp ), reg->sp, reg->f );
			break;
		case 0xD1:
			POP( getWordAt( reg->sp ) ,reg->sp, reg->de );
			break;
		case 0xD2:
			JPNC( reg->pc, getNextWord(), reg->f );
			break;
		case 0xD3:
			OUTA( &IOport[ *( getNextByte() ) ], reg->a );
			break;
		case 0xD4:
			CALLNC( getNextWord(), getWordAt( reg->sp ), reg->sp, reg->pc, reg->f  );
			break;
		case 0xD5:
			PUSH( getWordAt( reg->sp ), reg->sp, reg->de );
			break;
		case 0xD6:
			SUB( reg->a, getNextByte(), reg->f );
			break;
		case 0xD7:
			RST( 0x10, getWordAt( reg->sp ), reg->sp, reg->pc );
			break;
		case 0xD8:
			RETC( reg->pc, getWordAt( reg->sp ), reg->sp, reg->f );
			break;
		case 0xD9:
			t_counter += 4;
			EXX( reg->bc, reg->de, reg->hl, reg->altbc, reg->altde, reg->althl );
			break;
		case 0xDA:
			JPC( reg->pc, getNextWord(), reg->f );
			break;
		case 0xDB:
			INA( reg->a, readPort(0x00FE) );
			break;
		case 0xDC:
			CALLC( getNextWord(), getWordAt( reg->sp ), reg->sp, reg->pc, reg->f  );
			break;
		case 0xDD: /* IX registers */
			execute_multi_byte_opcode_DD( opcode );
		case 0xDE:
			SBC( reg->a, getNextByte(), reg->f );
			break;
		case 0xDF:
			RST( 0x18, getWordAt( reg->sp ), reg->sp, reg->pc );
			break;
		case 0xE0:
			RETPO( reg->pc, getWordAt( reg->sp ), reg->sp, reg->f );
			break;
		case 0xE1:
			POP( getWordAt( reg->sp ) ,reg->sp, reg->hl );
			break;
		case 0xE2:
			JPPO( reg->pc, getNextWord(), reg->f );
			break;
		case 0xE3:
			EX( getWordAt( reg->sp ), reg->hl );
			break;
		case 0xE4:
			CALLPO( getNextWord(), getWordAt( reg->sp ), reg->sp, reg->pc, reg->f  );
			break;
		case 0xE5:
			PUSH( getWordAt( reg->sp ), reg->sp, reg->hl );
			break;
		case 0xE6:
			AND( reg->a, getNextByte(), reg->f );
			break;
		case 0xE7:
			RST( 0x20, getWordAt( reg->sp ), reg->sp, reg->pc );
			break;
		case 0xE8:
			RETPE( reg->pc, getWordAt( reg->sp ), reg->sp, reg->f  );
			break;
		case 0xE9:
			JP( reg->pc, reg->hl );
			break;
		case 0xEA:
			JPPE( reg->pc, getNextWord(), reg->f );
			break;
		case 0xEB:
			EX( reg->de, reg->hl );
			break;
		case 0xEC:
			CALLPE( getNextWord(), getWordAt( reg->sp ), reg->sp, reg->pc, reg->f  );
			break;
		case 0xED:
			execute_multi_byte_opcode_ED( opcode );
			break;
		case 0xEE:
			XOR( reg->a, getNextByte(), reg->f );
			break;
		case 0xEF:
			RST( 0x28, getWordAt( reg->sp ), reg->sp, reg->pc );
			break;
		case 0xF0:
			RETP( reg->pc, getWordAt( reg->sp ), reg->sp, reg->f  );
			break;
		case 0xF1:
			POP( getWordAt( reg->sp ) ,reg->sp, reg->af );
			break;
		case 0xF2:
			JPP( reg->pc, getNextWord(), reg->f  );
			break;
		case 0xF3:
			DI( reg->iff1, reg->iff2 );
			break;
		case 0xF4:
			CALLP( getNextWord(), getWordAt( reg->sp ), reg->sp, reg->pc, reg->f );
			break;
		case 0xF5:
			PUSH( getWordAt( reg->sp ), reg->sp, reg->af );
			break;
		case 0xF6:
			OR( reg->a, getNextByte(), reg->f );
			break;
		case 0xF7:
			RST( 0x30, getWordAt( reg->sp ), reg->sp, reg->pc );
			break;
		case 0xF8:
			RETM( reg->pc, getWordAt( reg->sp ), reg->sp, reg->f );
			break;
		case 0xF9:
			LD16( reg->sp, reg->hl );
			break;
		case 0xFA:
			JPM( reg->pc, getNextWord(), reg->f );
			break;
		case 0xFB:
			EI( reg->iff1, reg->iff2 );
			break;
		case 0xFC:
			CALLM( getNextWord(), getWordAt( reg->sp ), reg->sp, reg->pc, reg->f  );
			break;
		case 0xFD:
			execute_multi_byte_opcode_FD( opcode );
		default:
			break;
	}
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
			RLC( reg->b, reg->f );
			break;
		case 0x01:
			RLC( reg->c, reg->f );
			break;
		case 0x02:
			RLC( reg->d, reg->f );
			break;
		case 0x03:
			RLC( reg->e, reg->f );
			break;
		case 0x04:
			RLC( reg->h, reg->f );
			break;
		case 0x05:
			RLC( reg->l, reg->f );
			break;
		case 0x06:
			RLC( getByteAt( *reg->hl ), reg->f );
			break;
		case 0x07:
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
			SRL( reg->h, reg->f );
			break;
		case 0x3D:
			SRL( reg->l, reg->f );
			break;
		case 0x3E:
			break;
		case 0x3F:
			break;
		case 0x40:
			BIT( 0, reg->b, reg->f );
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
			BIT( 2, getByteAt( *reg->hl ), reg->f );
			break;
		case 0x57:
			break;
		case 0x58:
			break;
		case 0x59:
			break;
		case 0x5A:
			BIT( 3, reg->d, reg->f );
			break;
		case 0x5B:
			break;
		case 0x5C:
			break;
		case 0x5D:
			break;
		case 0x5E:
			BIT( 3, getByteAt( *reg->hl ), reg->f );
			break;
		case 0x5F:
			break;
		case 0x60:
			BIT( 4, reg->b, reg->f );
			break;
		case 0x61:
			BIT( 4, reg->c, reg->f );
			break;
		case 0x62:
			BIT( 4, reg->d, reg->f );
			break;
		case 0x63:
			BIT( 4, reg->e, reg->f );
			break;
		case 0x64:
			BIT( 4, reg->h, reg->f );
			break;
		case 0x65:
			BIT( 4, reg->l, reg->f );
			break;
		case 0x66:
			BIT( 4, getByteAt( *reg->hl ), reg->f );
			break;
		case 0x67:
			BIT( 4, reg->a, reg->f );
			break;
		case 0x6E:
			BIT( 5, getByteAt( *reg->hl ), reg->f );
			break;
		case 0x76:
			BIT( 6, getByteAt( *reg->hl ), reg->f );
			break;
		case 0x7E:
			BIT( 7, getByteAt( *reg->hl ), reg->f );
			break;
		case 0x86:
			RES( 0, getByteAt( *reg->hl ) );
			break;
		case 0x8E:
			RES( 1, getByteAt( *reg->hl ) );
			break;
		case 0x96:
			RES( 2, getByteAt( *reg->hl ) );
			break;
		case 0x9E:
			RES( 3, getByteAt( *reg->hl ) );
			break;
		case 0xA6:
			RES( 4, getByteAt( *reg->hl ) );
			break;
		case 0xAE:
			RES( 5, getByteAt( *reg->hl ) );
			break;
		case 0xB6:
			RES( 6, getByteAt( *reg->hl ) );
			break;
		case 0xBC:
			RES( 7, reg->h );
			break;
		case 0xBE:
			RES( 7, getByteAt( *reg->hl ) );
			break;
		case 0xC6:
			SET( 0, getByteAt( *reg->hl ) );
			break;
		case 0xCE:
			SET( 1, getByteAt( *reg->hl ) );
			break;
		case 0xD6:
			SET( 2, getByteAt( *reg->hl ) );
			break;
		case 0xDE:
			SET( 3, getByteAt( *reg->hl ) );
			break;
		case 0xE6:
			SET( 4, getByteAt( *reg->hl ) );
			break;
		case 0xEE:
			SET( 5, getByteAt( *reg->hl ) );
			break;
		case 0xF6:
			SET( 6, getByteAt( *reg->hl ) );
			break;
		case 0xFD:
			SET( 7, reg->l );
			break;
		case 0xFE:
			SET( 7, getByteAt( *reg->hl ) );
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
			ADD16( reg->ix, reg->bc ,reg->f );
			break;
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
			INC16( reg->ix );
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
		uint8_t offset = *( getNextByte() );
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
			printf( "JP IX" );
			JP( reg->pc, reg->ix );
			break;
		case 0xF9:
			printf( "LD SP,IX" );
			LD16( reg->sp, reg->ix );
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
			t_counter += 10;
			LD16( reg->bc, getWordAt( getNextWord() ) );
			break;
		case 0x4F:
			printf( "LD R,A");
			LD( reg->r, reg->a );
			break;
		case 0x52:
			printf( "SBC HL,DE");
			t_counter += 15;
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
			t_counter += 20;
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

			IN( reg->a, reg->b, (*portPtrs[254])( READ, *reg->b )/*readPort( port )*/, reg->f );
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
}

static void execute_multi_byte_opcode_FD( uint8_t *opcode )
{
	switch( *(getNextByte()) )
	{
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
		case 0x56:
			printf( "LD D,(IY+%X)",(int8_t) readNextByte() );
			LD( reg->d, getByteAt( *reg->iy +(int8_t) *( getNextByte() ) ) );
			break;
		case 0x5E:
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
			uint8_t offset = *( getNextByte() );
			switch( *( getNextByte() ) )
			{
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
