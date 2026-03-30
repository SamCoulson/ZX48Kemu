#include "../include/cpu.h"
#include "../include/16-bit_arithmetic_group.h"
#include "../include/16-bit_load_group.h"
#include "../include/8-bit_arithmetic_group.h"
#include "../include/8-bit_load_group.h"
#include "../include/BSR_and_test_group.h"
#include "../include/EBTS_group.h"
#include "../include/GPA_and_CPU_control_group.h"
#include "../include/IO_group.h"
#include "../include/call_and_return_group.h"
#include "../include/jump_group.h"
#include "../include/memory.h"
#include "../include/rotate_and_shift_group.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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

// 8-Bit Z80 main general purpose z80 register
union _AF
{
    uint8_t _af[2]; // Accumulater	// Flags z80ister (Set = 1, Reset = 0)
    uint16_t af;
} af;

union _BC
{
    uint8_t _bc[2]; // Byte counter
    uint16_t bc;
} bc;

union _DE
{
    uint8_t _de[2];
    uint16_t de;
} de;

union _HL
{
    uint8_t _hl[2];
    uint16_t hl;
} hl;

union _ALTAF
{
    uint8_t _af[2]; // Accumulater	// Flags z80ister (Set = 1, Reset = 0)
    uint16_t af;
} altaf;

union _ALTBC
{
    uint8_t _bc[2]; // Byte counter
    uint16_t bc;
} altbc;

union _ALTDE
{
    uint8_t _de[2];
    uint16_t de;
} altde;

union _ALTHL
{
    uint8_t _hl[2];
    uint16_t hl;
} althl;
// 8-Bit special z80 register - Interrupt vector
union _IR
{
    uint8_t _ir[2];
    uint16_t ir;
} ir;

// Declare and initialise structure of pointers to z80 register unions, this
// must be here not in the .h file
Z80 _z80 = {
    // Assign const pointers to main z80 reg register
    .a = &af._af[1],
    .f = &af._af[0],
    .af = &af.af,
    .b = &bc._bc[1],
    .c = &bc._bc[0],
    .bc = &bc.bc,
    .d = &de._de[1],
    .e = &de._de[0],
    .de = &de.de,
    .h = &hl._hl[1],
    .l = &hl._hl[0],
    .hl = &hl.hl,
    // Assign const pointers to alternative z80 register
    .alta = &altaf._af[1],
    .altf = &altaf._af[0],
    .altaf = &altaf.af,
    .altb = &altbc._bc[1],
    .altc = &altbc._bc[0],
    .altbc = &altbc.bc,
    .altd = &altde._de[1],
    .alte = &altde._de[0],
    .altde = &altde.de,
    .alth = &althl._hl[1],
    .altl = &althl._hl[0],
    .althl = &althl.hl,
    // 8-bit Special z80 register - interrupt vector and refresh z80ister
    .i = &ir._ir[1],
    .r = &ir._ir[0],
    .ir = &ir.ir,
    // 16-bit index z80 register
    .ix = &ix,
    .iy = &iy,
    // 16-bit stack pointer and program pointer
    .sp = &sp,
    .pc = &pc,
    // IFF (Interrupt enabled flip-flop)
    .iff1 = &iff1,
    .iff2 = &iff2,
    // CPU Mode
    .mode = &mode};

// Pointer to the z80 register structure
Z80 *z80 = &_z80;

// IO ports *** The CPU has 256 addressable ports, using bits 0-7 of the address
// bus *** All IO routines will park data at these location for read/write by
// external modules

uint8_t ports[256] = {0};
// static uint8_t *IOport = ports;

// List of function pointers for each of the 256 ports
// Each function (port) takes a IO status i.e. READ or WRITE and an optional
// address which can be used to use the upper-part of the address bus
uint8_t (*portPtrs[256])(int, uint8_t);

static void execute_single_byte_opcode(uint8_t *opcode);
static void execute_multi_byte_opcode(uint8_t *opcode);
static void execute_multi_byte_opcode_CB(uint8_t *opcode);
static void execute_multi_byte_opcode_DD(uint8_t *opcode);
static void execute_multi_byte_opcode_ED(uint8_t *opcode);
static void execute_multi_byte_opcode_FD(uint8_t *opcode);

// Takes a port number and maps a function to it.
void mapPort(unsigned int port, uint8_t (*func)(int, uint8_t))
{

    if (port > 255)
    {
        printf("mapPort : Unable to map port, port number out of range\n");
        return;
    }

    if (func == NULL)
    {
        printf("mapPort : Unable to map port, func passed in is NULL\n");
        return;
    }

    portPtrs[port] = func;
}

void execute(uint8_t *opcode)
{
    // printf("entered execute\n");
    // printf("0x%04X\t%02X\t", *z80->pc, memory[*z80->pc]);

    ++*z80->r;

    if (*opcode == 0xCB || *opcode == 0xDD || *opcode == 0xED ||
        *opcode == 0xFD)
    {
        // printf("executing multi byte opcode %X\n", *opcode);
        execute_multi_byte_opcode(opcode);
    }
    else if (*opcode >= 0x00 && *opcode <= 0xFF)
    {
        // printf("executing single byte opcode\n");

        execute_single_byte_opcode(opcode);

        // printf("executed single byte opcode\n");
    }

    assert(*opcode >= 0x00 && *opcode <= 0xFF);
}

void execute_single_byte_opcode(uint8_t *opcode)
{
    // printf("running instr 0x%X\n", *opcode);

    z80_instruction instruction = single_byte_instruction_lookup[*opcode];

    uint8_t t_cycles = instruction.func(z80);

    // printf("finished running instr 0x%X\n", *opcode);
    /*switch( *opcode )*/
    /*{*/
    /**/
    /*//	case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case
     * 0x45: case 0x47: case 0x48: case 0x49: case 0x4A: case 0x4B: case 0x4C:
     * case 0x4D: case 0x4F:*/
    /*//	case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case
     * 0x55: case 0x57: case 0x58: case 0x59: case 0x5A: case 0x5B: case 0x5C:
     * case 0x5D: case 0x5F: */
    /*//	case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case
     * 0x65: case 0x67: case 0x68: case 0x69: case 0x6A: case 0x6B: case 0x6C:
     * case 0x6D: case 0x6F: */
    /*//	case 0x78: case 0x79: case 0x7A: case 0x7B: case 0x7C: case
     * 0x7D: case 0x7F:*/
    /**/
    /*	//break;*/
    /*	case 0x00:*/
    /*		NI();*/
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
    /*		break;*/
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
    /*		break;*/
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
    /*		CALLNZ( getNextWord(), getWordAt( z80->sp ), z80->sp, z80->pc,
     * z80->f );*/
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
    /*		CALLZ( getNextWord(), getWordAt( z80->sp ), z80->sp, z80->pc,
     * z80->f );*/
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
    /*		CALLNC( getNextWord(), getWordAt( z80->sp ), z80->sp, z80->pc,
     * z80->f  );*/
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
    /*		EXX( z80->bc, z80->de, z80->hl, z80->altbc, z80->altde,
     * z80->althl );*/
    /*		break;*/
    /*	case 0xDA:*/
    /*		JPC( z80->pc, getNextWord(), z80->f );*/
    /*		break;*/
    /*	case 0xDB:*/
    /*		INA( z80->a, readPort(0x00FE) );*/
    /*		break;*/
    /*	case 0xDC:*/
    /*		CALLC( getNextWord(), getWordAt( z80->sp ), z80->sp, z80->pc,
     * z80->f  );*/
    /*		break;*/
    /*	case 0xDD:  IX z80 register */
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
    /*		CALLPO( getNextWord(), getWordAt( z80->sp ), z80->sp, z80->pc,
     * z80->f  );*/
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
    /*		CALLPE( getNextWord(), getWordAt( z80->sp ), z80->sp, z80->pc,
     * z80->f  );*/
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
    /*		CALLP( getNextWord(), getWordAt( z80->sp ), z80->sp, z80->pc,
     * z80->f );*/
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
    /*		CALLM( getNextWord(), getWordAt( z80->sp ), z80->sp, z80->pc,
     * z80->f  );*/
    /*		break;*/
    /*	case 0xFD:*/
    /*		execute_multi_byte_opcode_FD( opcode );*/
    /*	default:*/
    /*		break;*/
    /*}*/
}

void execute_multi_byte_opcode(uint8_t *opcode)
{
    switch (*opcode)
    {
    case 0xCB:
        execute_multi_byte_opcode_CB(opcode);
        break;
    case 0xDD:
        execute_multi_byte_opcode_DD(opcode);
        break;
    case 0xED:
        uint8_t op = *getNextByte();
        // printf("executing instruction %X\n", op);
        z80_instruction instruction = ed_multi_byte_instruction_lookup[op];
        uint8_t t_cycles = instruction.func(z80);
        // execute_multi_byte_opcode_ED(opcode);
        break;
    case 0xFD:
        execute_multi_byte_opcode_FD(opcode);
        break;
    default:
        break;
    }
}

void execute_multi_byte_opcode_CB(uint8_t *opcode)
{
    switch (*(getNextByte()))
    {
    case 0x00:
        RLC(z80->b, z80->f);
        break;
    case 0x01:
        RLC(z80->c, z80->f);
        break;
    case 0x02:
        RLC(z80->d, z80->f);
        break;
    case 0x03:
        RLC(z80->e, z80->f);
        break;
    case 0x04:
        RLC(z80->h, z80->f);
        break;
    case 0x05:
        RLC(z80->l, z80->f);
        break;
    case 0x06:
        RLC(getByteAt(*z80->hl), z80->f);
        break;
    case 0x07:
        RLC(z80->a, z80->f);
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
        SRL(z80->h, z80->f);
        break;
    case 0x3D:
        SRL(z80->l, z80->f);
        break;
    case 0x3E:
        break;
    case 0x3F:
        break;
    case 0x40:
        BIT(0, z80->b, z80->f);
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
        BIT(0, getByteAt(*z80->hl), z80->f);
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
        BIT(1, getByteAt(*z80->hl), z80->f);
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
        BIT(2, getByteAt(*z80->hl), z80->f);
        break;
    case 0x57:
        break;
    case 0x58:
        break;
    case 0x59:
        break;
    case 0x5A:
        BIT(3, z80->d, z80->f);
        break;
    case 0x5B:
        break;
    case 0x5C:
        break;
    case 0x5D:
        break;
    case 0x5E:
        BIT(3, getByteAt(*z80->hl), z80->f);
        break;
    case 0x5F:
        break;
    case 0x60:
        BIT(4, z80->b, z80->f);
        break;
    case 0x61:
        BIT(4, z80->c, z80->f);
        break;
    case 0x62:
        BIT(4, z80->d, z80->f);
        break;
    case 0x63:
        BIT(4, z80->e, z80->f);
        break;
    case 0x64:
        BIT(4, z80->h, z80->f);
        break;
    case 0x65:
        BIT(4, z80->l, z80->f);
        break;
    case 0x66:
        BIT(4, getByteAt(*z80->hl), z80->f);
        break;
    case 0x67:
        BIT(4, z80->a, z80->f);
        break;
    case 0x6E:
        BIT(5, getByteAt(*z80->hl), z80->f);
        break;
    case 0x76:
        BIT(6, getByteAt(*z80->hl), z80->f);
        break;
    case 0x7E:
        BIT(7, getByteAt(*z80->hl), z80->f);
        break;
    case 0x86:
        RES(0, getByteAt(*z80->hl));
        break;
    case 0x8E:
        RES(1, getByteAt(*z80->hl));
        break;
    case 0x96:
        RES(2, getByteAt(*z80->hl));
        break;
    case 0x9E:
        RES(3, getByteAt(*z80->hl));
        break;
    case 0xA6:
        RES(4, getByteAt(*z80->hl));
        break;
    case 0xAE:
        RES(5, getByteAt(*z80->hl));
        break;
    case 0xB6:
        RES(6, getByteAt(*z80->hl));
        break;
    case 0xBC:
        RES(7, z80->h);
        break;
    case 0xBE:
        RES(7, getByteAt(*z80->hl));
        break;
    case 0xC6:
        SET(0, getByteAt(*z80->hl));
        break;
    case 0xCE:
        SET(1, getByteAt(*z80->hl));
        break;
    case 0xD6:
        SET(2, getByteAt(*z80->hl));
        break;
    case 0xDE:
        SET(3, getByteAt(*z80->hl));
        break;
    case 0xE6:
        SET(4, getByteAt(*z80->hl));
        break;
    case 0xEE:
        SET(5, getByteAt(*z80->hl));
        break;
    case 0xF6:
        SET(6, getByteAt(*z80->hl));
        break;
    case 0xFD:
        SET(7, z80->l);
        break;
    case 0xFE:
        SET(7, getByteAt(*z80->hl));
        break;
    default:
        break;
    }
}

void execute_multi_byte_opcode_DD(uint8_t *opcode)
{
    switch (*(getNextByte()))
    {
    case 0x09:
        printf("ADD IX + BC");
        ADD16(z80->ix, z80->bc, z80->f);
        break;
    case 0x21:
        printf("LD IX,+%X", readNextWord());
        LD16(z80->ix, getNextWord());
        break;
    case 0x22:
        LD16(getWordAt(getNextWord()), z80->ix);
        break;
    case 0x23:
        printf("INC IX");
        INC16(z80->ix);
        break;
    case 0x2A:
        printf("LD IX(%X)", readNextWord());
        LD16(z80->ix, getWordAt(getNextWord()));
        break;
    case 0x2B:
        printf("DEC IX");
        DEC16(z80->ix);
        break;
    case 0x34:
        printf("INC IX+%X", (int8_t)readNextByte());
        INC(getByteAt(*z80->ix + (int8_t)*(getNextByte())), z80->f);
        break;
    case 0x35:
        printf("DEC IX +%X", (int8_t)readNextByte());
        DEC(getByteAt(*z80->ix + (int8_t)*(getNextByte())), z80->f);
        break;
    case 0x36:
        printf("LD (IX+%X),+%X", (int8_t)readNextByte(),
               readNextByte() + 1); // BROKEN second param is incorrect
        LD(getByteAt(*z80->ix + (int8_t)*(getNextByte())), getNextByte());
        break;
    case 0x46:
        printf("LD B,(IX+%X)", (int8_t)readNextByte());
        LD(z80->b, getByteAt(*z80->ix + (int8_t)*(getNextByte())));
        break;
    case 0x4E:
        printf("LD C,(IX+%X)", (int8_t)readNextByte());
        LD(z80->c, getByteAt(*z80->ix + (int8_t)*(getNextByte())));
        break;
    case 0x5E:
        printf("LD D,(IX+%X)", (int8_t)readNextByte());
        LD(z80->d, getByteAt(*z80->ix + (int8_t)*(getNextByte())));
        break;
    case 0x56:
        printf("LD E,(IX+%X)", (int8_t)readNextByte());
        LD(z80->e, getByteAt(*z80->ix + (int8_t)*(getNextByte())));
        break;
    case 0x66:
        printf("LD H,(IX+%X)", (int8_t)readNextByte());
        LD(z80->h, getByteAt(*z80->ix + (int8_t)*(getNextByte())));
        break;
    case 0x6E:
        printf("LD L,(IX+%X)", (int8_t)readNextByte());
        LD(z80->l, getByteAt(*z80->ix + (int8_t)*(getNextByte())));
        break;
    case 0x70:
        printf("LD (IX+%X),B", (int8_t)readNextByte());
        LD(getByteAt(*z80->ix + (int8_t)*(getNextByte())), z80->b);
        break;
    case 0x71:
        printf("LD (IX+%X),C", (int8_t)readNextByte());
        LD(getByteAt(*z80->ix + (int8_t)*(getNextByte())), z80->c);
        break;
    case 0x72:
        printf("LD (IX+%X),D", (int8_t)readNextByte());
        LD(getByteAt(*z80->ix + (int8_t)*(getNextByte())), z80->d);
        break;
    case 0x73:
        printf("LD (IX+%X),E", (int8_t)readNextByte());
        LD(getByteAt(*z80->ix + (int8_t)*(getNextByte())), z80->e);
        break;
    case 0x74:
        printf("LD (IX+%X),H", (int8_t)readNextByte());
        LD(getByteAt(*z80->ix + (int8_t)*(getNextByte())), z80->h);
        break;
    case 0x75:
        printf("LD (IX+%X),L", (int8_t)readNextByte());
        LD(getByteAt(*z80->ix + (int8_t)*(getNextByte())), z80->l);
        break;
    case 0x77:
        printf("LD (IX+%X),A", (int8_t)readNextByte());
        LD(getByteAt(*z80->ix + (int8_t)*(getNextByte())), z80->a);
        break;
    case 0x7E:
        printf("LD A,(IX+%X)", (int8_t)readNextByte());
        LD(z80->a, getByteAt(*z80->ix + (int8_t)*(getNextByte())));
        break;
    case 0x86:
        printf("ADD (IX+%X)", (int8_t)readNextByte());
        ADD(z80->a, getByteAt(*z80->ix + (int8_t)*(getNextByte())), z80->f);
        break;
    case 0x8E:
        printf("ADC (IX+%X)", (int8_t)readNextByte());
        ADC(z80->a, getByteAt(*z80->ix + (int8_t)*(getNextByte())), z80->f);
        break;
    case 0x96:
        printf("SUB (IX+%X)", (int8_t)readNextByte());
        SUB(z80->a, getByteAt(*z80->ix + (int8_t)*(getNextByte())), z80->f);
        break;
    case 0xA6:
        printf("AND A (IX+%X)", (int8_t)readNextByte());
        AND(z80->a, getByteAt(*z80->ix + (int8_t)*(getNextByte())), z80->f);
        break;
    case 0xAE:
        printf("XOR (IX+%X)", (int8_t)readNextByte());
        XOR(z80->a, getByteAt(*z80->ix + (int8_t)*(getNextByte())), z80->f);
        break;
    case 0xB6:
        printf("OR A,(IX+%X)", (int8_t)readNextByte());
        OR(z80->a, getByteAt(*z80->ix + (int8_t)*(getNextByte())), z80->f);
        break;
    case 0xBE:
        printf("CP (IX+%X)", (int8_t)readNextByte());
        CP(*z80->a, *getByteAt(*z80->ix + (int8_t)*(getNextByte())), z80->f);
        break;
    case 0xCB:
        uint8_t offset = *(getNextByte());
        switch (*(getNextByte()))
        {
        case 0x06:
            printf("RLC(IX+%X)", offset);
            RLC(getByteAt(*z80->ix + (int8_t)*(getNextByte())), z80->f);
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
            printf("BIT 0, (IX+%X)", (int8_t)offset);
            BIT(0, getByteAt(*z80->ix + (int8_t)offset), z80->f);
            break;
        case 0x4E:
            printf("BIT 1, (IX+%X)", (int8_t)offset);
            BIT(1, getByteAt(*z80->ix + (int8_t)offset), z80->f);
            break;
        case 0x56:
            printf("BIT 2, (IX+%X)", (int8_t)offset);
            BIT(2, getByteAt(*z80->ix + (int8_t)offset), z80->f);
            break;
        case 0x5E:
            printf("BIT 3, (IX+%X)", (int8_t)offset);
            BIT(3, getByteAt(*z80->ix + (int8_t)offset), z80->f);
            break;
        case 0x66:
            printf("BIT 4, (IX+%X)", (int8_t)offset);
            BIT(4, getByteAt(*z80->ix + (int8_t)offset), z80->f);
            break;
        case 0x6E:
            printf("BIT 5, (IX+%X)", (int8_t)offset);
            BIT(5, getByteAt(*z80->ix + (int8_t)offset), z80->f);
            break;
        case 0x76:
            printf("BIT 6, (IX+%X)", (int8_t)offset);
            BIT(6, getByteAt(*z80->ix + (int8_t)offset), z80->f);
            break;
        case 0x7E:
            printf("BIT 7, (IX+%X)", (int8_t)offset);
            BIT(7, getByteAt(*z80->ix + (int8_t)offset), z80->f);
            break;
        case 0x86:
            printf("RES 0, (IX+%X)", (int8_t)offset);
            RES(0, getByteAt(*z80->ix + (int8_t)offset));
            break;
        case 0x8E:
            printf("RES 1, (IX+%X)", (int8_t)offset);
            RES(1, getByteAt(*z80->ix + (int8_t)offset));
            break;
        case 0x96:
            printf("RES 2, (IX+%X)", (int8_t)offset);
            RES(2, getByteAt(*z80->ix + (int8_t)offset));
            break;
        case 0x9E:
            printf("RES 3, (IX+%X)", (int8_t)offset);
            RES(3, getByteAt(*z80->ix + (int8_t)offset));
            break;
        case 0xA6:
            printf("RES 4, (IX+%X)", (int8_t)offset);
            RES(4, getByteAt(*z80->ix + (int8_t)offset));
            break;
        case 0xAE:
            printf("RES 5, (IX+%X)", (int8_t)offset);
            RES(5, getByteAt(*z80->ix + (int8_t)offset));
            break;
        case 0xB6:
            printf("RES 6, (IX+%X)", (int8_t)offset);
            RES(6, getByteAt(*z80->ix + (int8_t)offset));
            break;
        case 0xBE:
            printf("RES 7, (IX+%X)", (int8_t)offset);
            RES(7, getByteAt(*z80->ix + (int8_t)offset));
            break;
        case 0xC6:
            printf("SET 0, (IX+%X)", (int8_t)offset);
            SET(0, getByteAt(*z80->ix + (int8_t)offset));
            break;
        case 0xCE:
            printf("SET 1, (IX+%X)", (int8_t)offset);
            SET(1, getByteAt(*z80->ix + (int8_t)offset));
            break;
        case 0xD6:
            printf("SET 2, (IX+%X)", (int8_t)offset);
            SET(2, getByteAt(*z80->ix + (int8_t)offset));
            break;
        case 0xDE:
            printf("SET 3, (IX+%X)", (int8_t)offset);
            SET(3, getByteAt(*z80->ix + (int8_t)offset));
            break;
        case 0xE6:
            printf("SET 4, (IX+%X)", (int8_t)offset);
            SET(4, getByteAt(*z80->ix + (int8_t)offset));
            break;
        case 0xEE:
            printf("SET 5, (IX+%X)", (int8_t)offset);
            SET(5, getByteAt(*z80->ix + (int8_t)offset));
            break;
        case 0xF6:
            printf("SET 6, (IX+%X)", (int8_t)offset);
            SET(6, getByteAt(*z80->ix + (int8_t)offset));
            break;
        case 0xFE:
            printf("SET 7, (IX+%X)", (int8_t)offset);
            SET(7, getByteAt(*z80->ix + (int8_t)offset));
            break;
        default:
            break;
        }
        break;
    case 0xE1:
        printf("POP IX");
        POP(getWordAt(z80->sp), z80->sp, z80->ix);
        break;
    case 0xE3:
        printf("EX (SP),IX");
        EX(getWordAt(z80->sp), z80->ix);
        break;
    case 0xE5:
        printf("PUSH IX");
        PUSH(getWordAt(z80->sp), z80->sp, z80->ix);
        break;
    case 0xE9:
        // printf( "JP IX" );
        JP(z80->pc, z80->ix);
        break;
    case 0xF9:
        printf("LD SP,IX");
        LD16(z80->sp, z80->ix);
        break;
    default:
        break;
    }
}

void execute_multi_byte_opcode_ED(uint8_t *opcode)
{
    switch (*(getNextByte()))
    {
    case 0x42:
        printf("SBC HL,BC");
        SBC16(z80->hl, z80->bc, z80->f);
        break;
    case 0x43:
        printf("LD (%X),BC", readNextWord());
        LD16(getWordAt(getNextWord()), z80->bc);
        break;
    case 0x46:
        printf("IM 0");
        IM0(z80->mode);
        break;
    case 0x47:
        printf("LD I,A");
        LD(z80->i, z80->a);
        *z80->pc += 2;
        break;
    case 0x4B:
        printf("LD BC,(%X)", readNextWord());
        t_counter += 10;
        LD16(z80->bc, getWordAt(getNextWord()));
        break;
    case 0x4F:
        printf("LD R,A");
        LD(z80->r, z80->a);
        break;
    case 0x52:
        printf("SBC HL,DE\n");
        t_counter += 15;
        SBC16(z80->hl, z80->de, z80->f);
        ++*z80->pc;
        break;
    case 0x53:
        printf("LD (%X),DE", readNextWord());
        LD16(getWordAt(getNextWord()), z80->de);
        break;
    case 0x56:
        printf("IM 1");
        IM1(z80->mode);
        break;
    case 0x57:
        printf("LD A,I");
        LDAIR(z80->a, z80->i, z80->f, z80->iff2);
        break;
    case 0x5B:
        printf("LD DE,(%X)", readNextWord());
        t_counter += 20;
        LD16(z80->de, getWordAt(getNextWord()));
        break;
    case 0x5E:
        printf("IM 2");
        IM2(z80->mode);
        break;
    case 0x5F:
        printf("LD A,R");
        LDAIR(z80->a, z80->r, z80->f, z80->iff2);
        break;
    case 0x62:
        printf("SBC HL,HL");
        SBC16(z80->hl, z80->hl, z80->f);
        break;
    case 0x72:
        printf("SBC HL,SP");
        SBC16(z80->hl, z80->sp, z80->f);
        break;
    case 0x73:
        printf("LD(%X),SP", readNextWord());
        LD16(getNextWord(), z80->sp);
        break;
    case 0x78:
        printf("IN A,(C)");

        IN(z80->a, z80->b, (*portPtrs[254])(READ, *z80->b) /*readPort( port )*/,
           z80->f);
        break;
    case 0x7B:
        printf("LD SP,(%X)", readNextWord());
        LD16(z80->sp, getWordAt(getNextWord()));
        break; /*
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
        printf("LDIR");
        LDIR(getWordAt(z80->hl), getWordAt(z80->de), z80->hl, z80->de, z80->bc,
             z80->pc, z80->f);
        break; /*
 case 0xB1:
         //printf( "CPIR");
         CPIR();
         break;*/
    case 0xB8:
        printf("LDDR");
        LDDR(getWordAt(z80->hl), getWordAt(z80->de), z80->hl, z80->de, z80->bc,
             z80->f);
        break;
    case 0xB9:
        // printf( "CPDR");
        // CPDR();
        break;
    default:
        break;
    }
}

static void execute_multi_byte_opcode_FD(uint8_t *opcode)
{
    switch (*(getNextByte()))
    {
    case 0x21:
        printf("LD IY,+%X", readNextWord());
        LD16(z80->iy, getNextWord());
        break;
    case 0x22:
        printf("LD (%X),IY", readNextWord());
        LD16(getWordAt(getNextWord()), z80->iy);
        break;
    case 0x23:
        printf("INC IX");
        INC16(z80->ix);
        break;
    case 0x2A:
        printf("LD IY(%X)", readNextWord());
        LD16(z80->iy, getWordAt(getNextWord()));
        break;
    case 0x2B:
        printf("DEC IY");
        DEC16(z80->iy);
        break;
    case 0x34:
        printf("INC IY+%X", readNextByte());
        INC(getByteAt(*z80->iy + (int8_t)*(getNextByte())), z80->f);
        break;
    case 0x35:
        printf("DEC IY+%X", readNextByte());
        DEC(getByteAt(*z80->iy + (int8_t)*(getNextByte())), z80->f);
        break;
    case 0x36:
        printf("LD (IY+%X),%02X**", (int8_t)readNextByte(),
               readByteAt(*z80->pc + 2));
        LD(getByteAt(*z80->iy + (int8_t)*(getNextByte())),
           getByteAt(*z80->pc + 2));
        getNextByte();
        break;
    case 0x46:
        printf("LD B,(IY+%X)", (int8_t)readNextByte());
        LD(z80->b, getByteAt(*z80->iy + (int8_t)*(getNextByte())));
        break;
    case 0x4E:
        printf("LD C,(IY+%X)", (int8_t)readNextByte());
        LD(z80->c, getByteAt(*z80->iy + (int8_t)*(getNextByte())));
        break;
    case 0x56:
        printf("LD D,(IY+%X)", (int8_t)readNextByte());
        LD(z80->d, getByteAt(*z80->iy + (int8_t)*(getNextByte())));
        break;
    case 0x5E:
        printf("LD E,(IY+%X)", (int8_t)readNextByte());
        LD(z80->e, getByteAt(*z80->iy + (int8_t)*(getNextByte())));
        break;
    case 0x66:
        printf("LD H,(IY+%X)", (int8_t)readNextByte());
        LD(z80->h, getByteAt(*z80->iy + (int8_t)*(getNextByte())));
        break;
    case 0x6E:
        printf("LD L,(IY+%X)", (int8_t)readNextByte());
        LD(z80->l, getByteAt(*z80->iy + (int8_t)*(getNextByte())));
        break;
    case 0x70:
        printf("LD (IY+%X),B", (int8_t)readNextByte());
        LD(getByteAt(*z80->iy + (int8_t)*(getNextByte())), z80->b);
        break;
    case 0x71:
        printf("LD (IY+%X),C", (int8_t)readNextByte());
        LD(getByteAt(*z80->iy + (int8_t)*(getNextByte())), z80->c);
        break;
    case 0x72:
        printf("LD (IY+%X),D", (int8_t)readNextByte());
        LD(getByteAt(*z80->iy + (int8_t)*(getNextByte())), z80->d);
        break;
    case 0x73:
        printf("LD (IY+%X),E", (int8_t)readNextByte());
        LD(getByteAt(*z80->iy + (int8_t)*(getNextByte())), z80->e);
        break;
    case 0x74:
        printf("LD (IY+%X),H", (int8_t)readNextByte());
        LD(getByteAt(*z80->iy + (int8_t)*(getNextByte())), z80->h);
        break;
    case 0x75:
        printf("LD (IY+%X),L", (int8_t)readNextByte());
        LD(getByteAt(*z80->iy + (int8_t)*(getNextByte())), z80->l);
        break;
    case 0x77:
        printf("LD (IY+%X),A", (int8_t)readNextByte());
        LD(getByteAt(*z80->iy + (int8_t)*(getNextByte())), z80->a);
        break;
    case 0x7E:
        printf("LD A,(IY+%X", (int8_t)readNextByte());
        LD(z80->a, getByteAt(*z80->iy + (int8_t)*(getNextByte())));
        break;
    case 0x86:
        printf("ADD (IY+%X)", (int8_t)readNextByte());
        ADD(z80->a, getByteAt(*z80->iy + (int8_t)*(getNextByte())), z80->f);
        break;
    case 0x8E:
        printf("ADC (IY+%X)", (int8_t)readNextByte());
        ADC(z80->a, getByteAt(*z80->iy + (int8_t)*(getNextByte())), z80->f);
        break;
    case 0x96:
        printf("SUB (IY+%X)", (int8_t)readNextByte());
        SUB(z80->a, getByteAt(*z80->iy + (int8_t)*(getNextByte())), z80->f);
        break;
    case 0xA6:
        printf("AND A (IY+%X)", (int8_t)readNextByte());
        AND(z80->a, getByteAt(*z80->iy + (int8_t)*(getNextByte())), z80->f);
        break;
    case 0xAE:
        printf("XOR (IY+%X)", (int8_t)readNextByte());
        XOR(z80->a, getByteAt(*z80->iy + (int8_t)*(getNextByte())), z80->f);
        break;
    case 0xB6:
        printf("OR A,(IY+%X)", (int8_t)readNextByte());
        OR(z80->a, getByteAt(*z80->iy + (int8_t)*(getNextByte())), z80->f);
        break;
    case 0xBE:
        printf("CP (IY+%X)", (int8_t)readNextByte());
        CP(*z80->a, *getByteAt(*z80->iy + (int8_t)*(getNextByte())), z80->f);
        break;
    case 0xCB:
        uint8_t offset = *(getNextByte());
        switch (*(getNextByte()))
        {
        case 0x06:
            printf("RLC(IY+%X)", offset);
            RLC(getByteAt(*z80->iy + (int8_t)*(getNextByte())), z80->f);
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
            printf("BIT 0, (IY+%X)", (int8_t)offset);
            BIT(0, getByteAt(*z80->iy + (int8_t)offset), z80->f);
            break;
            break;
        case 0x4E:
            printf("BIT 1, (IY+%X)", (int8_t)offset);
            BIT(1, getByteAt(*z80->iy + (int8_t)offset), z80->f);
            break;
        case 0x56:
            printf("BIT 2, (IY+%X)", (int8_t)offset);
            BIT(2, getByteAt(*z80->iy + (int8_t)offset), z80->f);
            break;
        case 0x5E:
            printf("BIT 3, (IY+%X)", (int8_t)offset);
            BIT(3, getByteAt(*z80->iy + (int8_t)offset), z80->f);
            break;
        case 0x66:
            printf("BIT 4, (IY+%X)", (int8_t)offset);
            BIT(4, getByteAt(*z80->iy + (int8_t)offset), z80->f);
            break;
        case 0x6E:
            printf("BIT 5, (IY+%X)", (int8_t)offset);
            BIT(5, getByteAt(*z80->iy + (int8_t)offset), z80->f);
            break;
        case 0x76:
            printf("BIT 6, (IY+%X)", (int8_t)offset);
            BIT(6, getByteAt(*z80->iy + (int8_t)offset), z80->f);
            break;
        case 0x7E:
            printf("BIT 7, (IY+%X)", (int8_t)offset);
            BIT(7, getByteAt(*z80->iy + (int8_t)offset), z80->f);
            break;
        case 0x86:
            printf("RES 0, (IY+%X)", (int8_t)offset);
            RES(0, getByteAt(*z80->iy + (int8_t)offset));
            break;
        case 0x8E:
            printf("RES 1, (IY+%X)", (int8_t)offset);
            RES(1, getByteAt(*z80->iy + (int8_t)offset));
            break;
        case 0x96:
            printf("RES 2, (IY+%X)", (int8_t)offset);
            RES(2, getByteAt(*z80->iy + (int8_t)offset));
            break;
        case 0x9E:
            printf("RES 3, (IY+%X)", (int8_t)offset);
            RES(3, getByteAt(*z80->iy + (int8_t)offset));
            break;
        case 0xA6:
            printf("RES 4, (IY+%d)", (int8_t)offset);
            RES(4, getByteAt(*z80->iy + (int8_t)offset));
            break;
        case 0xAE:
            printf("RES 5, (IY+%X)", (int8_t)offset);
            RES(5, getByteAt(*z80->iy + (int8_t)offset));
            break;
        case 0xB6:
            printf("RES 6, (IY+%X)", (int8_t)offset);
            RES(6, getByteAt(*z80->iy + (int8_t)offset));
            break;
        case 0xBE:
            printf("RES 7, (IY+%X)", (int8_t)offset);
            RES(7, getByteAt(*z80->iy + (int8_t)offset));
            break;
        case 0xC6:
            printf("SET 0, (IY+%X)", (int8_t)offset);
            SET(0, getByteAt(*z80->iy + (int8_t)offset));
            break;
        case 0xCE:
            printf("SET 1, (IY+%X) ", (int8_t)offset);
            SET(1, getByteAt(*z80->iy + (int8_t)offset));
            break;
        case 0xD6:
            printf("SET 2, (IY+%X) ", (int8_t)offset);
            SET(2, getByteAt(*z80->iy + (int8_t)offset));
            break;
        case 0xDE:
            printf("SET 3, (IY+%X) ", (int8_t)offset);
            SET(3, getByteAt(*z80->iy + (int8_t)offset));
            break;
        case 0xE6:
            printf("SET 4, (IY+%X) ", (int8_t)offset);
            SET(4, getByteAt(*z80->iy + (int8_t)offset));
            break;
        case 0xEE:
            printf("SET 5, (IY+%X) ", (int8_t)offset);
            SET(5, getByteAt(*z80->iy + (int8_t)offset));
            break;
        case 0xF6:
            printf("SET 6, (IY+%X) ", (int8_t)offset);
            SET(6, getByteAt(*z80->iy + (int8_t)offset));
            break;
        case 0xFE:
            printf("SET 7, (IY+%X) ", (int8_t)offset);
            SET(7, getByteAt(*z80->iy + (int8_t)offset));
            break;
        default:
            break;
        }
        offset = 0x00;
        break;
    case 0xE1:
        printf("POP IY");
        POP(getWordAt(z80->sp), z80->sp, z80->iy);
        break;
    case 0xE3:
        printf("EX (SP),IY");
        EX(getWordAt(z80->sp), z80->iy);
        break;
    case 0xE5:
        printf("PUSH IY");
        PUSH(getWordAt(z80->sp), z80->sp, z80->iy);
        break;
    case 0xE9:
        printf("JP(IY)");
        JP(z80->pc, getWordAt(z80->iy));
        break;
    case 0xF9:
        printf("LD SP,IY");
        LD16(z80->sp, z80->iy);
        break;
    case 0xFE:
        printf("CP %X", readNextByte());
        CP(*z80->a, *getNextByte(), z80->f);
        break;
    case 0xFF:
        printf("RST 38H");
        RST(0x38, getWordAt(z80->sp), z80->sp, z80->pc);
        break;
    default:
        break;
    }
}

// Return the 16-bit value of two 8-bit combined z80 register
uint16_t byteToWord(uint8_t *byte1, uint8_t *byte2)
{
    uint16_t word = 0x00;

    word = (word | *byte1);

    word <<= 8;

    word = (word | *byte2);

    return word;
}

// Returns the LO byte of a word *could make into macro functions *
uint8_t getLOByte(uint16_t *word)
{
    return (*word ^
            0xff00); // XOR the word with 1111111100000000 to zeros the HO byte
}

// Returns the HO byte of a word
uint8_t getHOByte(uint16_t *word)
{
    return ((*word ^ 0xff) >>
            8); // XOR the word with 1111111100000000 to zeros the LO byte then
                // bit shift 8 places to the right
}

// Memory handling

// Read byte from memory given 16-bit address
uint8_t *getNextByte()
{
    // Dereference into the memory location and return the value, progress pc by
    // 1
    return getByte(++*z80->pc);
}

// Read the next byte along, does not progress pc
uint8_t readNextByte()
{
    // Dereference into the memory location and return the value, progress pc by
    // 1
    return *(getByte(*z80->pc + 1));
}

uint16_t *getNextWord()
{
    // potentially deprecate this function in favor of read next word as this
    // alters the PC which should not happend anymore Dereference into the
    // memory location and return the word of the next two bytes
    uint16_t *word = getWord(*z80->pc++);
    // Increment pc again to point at at the second byte in the word, ready for
    // next increment by execution loop
    ++*z80->pc;
    return word;
}

// read next word along, does not progress pc
uint16_t readNextWord()
{
    // Dereference into the memory location and return the word of the next to
    // bytes
    return *(getWord(*z80->pc + 1));
}

uint8_t *getByteAt(uint16_t addrs)
{

    uint8_t *byte = getByte(addrs);
    // Return a pointer to the byte at a given 16-bit memory address
    return byte;
}

uint16_t *getWordAt(uint16_t *addrs) { return getWord(*addrs); }
// Read word at memory location
uint16_t read_word_at(uint16_t addrs) { return read_word(addrs); }

// deprecate this method it gets the endianness wrong
uint16_t readWordAt(uint16_t *addrs) { return *(getWord(*addrs)); }

uint16_t readByteAt(uint16_t addrs) { return *(getByte(addrs)); }
// Write byte to memory given 16-bit address
void writeByte(uint16_t addrs, uint8_t val)
{
    // setByte(addrs, val);
}

// ** Used internally to read in data from ports and if needed the address
// bus
// **/ Read the port at the given 8-bit address on the lower-half of the
// address bus. Note this param is a WORD as the address the Bus is still
// 16-bits wide.

uint8_t readPort(uint16_t portAddrs)
{

    // If called from IN r,(C) then only need to be concerned with
    // lower-half port address 0x00FE
    if (portAddrs == 0x00FE)
    {
        return 0x1F;
    }

    // If keyboard ports
    switch (portAddrs)
    {
    case 0xFEFE:
        // return readKeyboard( 1 );
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

z80_instruction single_byte_instruction_lookup[] = {
    {"NOP", 1, NOP},                     /*0x00*/
    {"LD BC,+%X", 1, NI},                /*0x01*/
    {"LD(BC),A", 1, NI},                 /*0x02*/
    {"INC BC", 1, NI},                   /*0x03*/
    {"INC B", 1, NI},                    /*0x04*/
    {"DEC B", 1, NI},                    /*0x05*/
    {"LD B,+%X", 1, NI},                 /*0x06*/
    {"RLCA", 1, NI},                     /*0x07*/
    {"EX AF,AF'", 1, NI},                /*0x08*/
    {"ADD HL,BC", 1, NI},                /*0x09*/
    {"LD A,(BC)", 1, NI},                /*0x0A*/
    {"DEC BC", 1, NI},                   /*0x0B*/
    {"INC C", 1, NI},                    /*0x0C*/
    {"DEC C", 1, NI},                    /*0x0D*/
    {"LD C,+%X", 1, NI},                 /*0x0E*/
    {"RRCA", 1, NI},                     /*0x0F*/
    {"DJNZ %d", 1, NI},                  /*0x10*/
    {"LD DE,+%X", 3, LD_DE_16},          /*0x11*/
    {"LD(DE),A", 1, NI},                 /*0x12*/
    {"INC DE", 1, NI},                   /*0x13*/
    {"INC D", 1, NI},                    /*0x14*/
    {"DEC D", 1, NI},                    /*0x15*/
    {"LD D,+%X", 1, NI},                 /*0x16*/
    {"RLA", 1, NI},                      /*0x17*/
    {"JR %X", 1, NI},                    /*0x18*/
    {"ADD HL,DE", 1, ADD_HL_DE},         /*0x19*/
    {"LD A,(DE)", 1, NI},                /*0x1A*/
    {"DEC DE", 1, NI},                   /*0x1B*/
    {"INC E", 1, NI},                    /*0x1C*/
    {"DEC E", 1, NI},                    /*0x1D*/
    {"LD E,+%X", 1, NI},                 /*0x1E*/
    {"RRA", 1, NI},                      /*0x1F*/
    {"JR NZ,%d", 1, JRNZ},               /*0x20*/
    {"LD HL,+%X", 1, NI},                /*0x21*/
    {"LD (%X),HL", 1, NI},               /*0x22*/
    {"INC HL", 1, INC_HL},               /*0x23*/
    {"INC H", 1, NI},                    /*0x24*/
    {"DEC H", 1, NI},                    /*0x25*/
    {"LD H,+%X", 1, NI},                 /*0x26*/
    {"DAA", 1, NI},                      /*0x27*/
    {"JR Z,+%X", 2, JRZ},                /*0x28*/
    {"ADD HL,HL", 1, NI},                /*0x29*/
    {"LD HL,(%X)", 1, NI},               /*0x2A*/
    {"DEC HL", 1, DEC_HL},               /*0x2B*/
    {"INC L", 1, NI},                    /*0x2C*/
    {"DEC L", 1, NI},                    /*0x2D*/
    {"LD L,+%X", 1, NI},                 /*0x2E*/
    {"CPL", 1, NI},                      /*0x2F*/
    {"JR NC,%d", 2, JR_NC_n},            /*0x30*/
    {"LD SP,+%X", 1, NI},                /*0x31*/
    {"LD (%X),A", 1, NI},                /*0x32*/
    {"INC SP", 1, NI},                   /*0x33*/
    {"INC (HL)", 1, NI},                 /*0x34*/
    {"DEC (HL)", 1, DEC_AT_ADDR_HL},     /*0x35*/
    {"LD (HL),+%X", 1, LD_ADDR_AT_HL_N}, /*0x36*/
    {"SCF", 1, NI},                      /*0x37*/
    {"JR C,%d", 1, NI},                  /*0x38*/
    {"ADD HL,SP", 1, NI},                /*0x39*/
    {"LD A,(%X)", 1, NI},                /*0x3A*/
    {"DEC SP", 1, NI},                   /*0x3B*/
    {"INC A", 1, NI},                    /*0x3C*/
    {"DEC A", 1, NI},                    /*0x3D*/
    {"LD A,+%X", 2, LD_A_N},             /*0x3E*/
    {"CCF", 1, NI},                      /*0x3F*/
    {"LD B,B", 1, NI},                   /*0x40*/
    {"LD B,C", 1, NI},                   /*0x41*/
    {"LD B,D", 1, NI},                   /*0x42*/
    {"LD B,E", 1, NI},                   /*0x43*/
    {"LD B,H", 1, NI},                   /*0x44*/
    {"LD B,L", 1, NI},                   /*0x45*/
    {"LD B,(HL)", 1, NI},                /*0x46*/
    {"LD B,A", 1, LD_B_A},               /*0x47*/
    {"LD C,B", 1, NI},                   /*0x48*/
    {"LD C,C", 1, NI},                   /*0x49*/
    {"LD C,D", 1, NI},                   /*0x4A*/
    {"LD C,E", 1, NI},                   /*0x4B*/
    {"LD C,H", 1, NI},                   /*0x4C*/
    {"LD C,L", 1, NI},                   /*0x4D*/
    {"LD C,(HL)", 1, NI},                /*0x4E*/
    {"LD C,A", 1, NI},                   /*0x4F*/
    {"LD D,B", 1, NI},                   /*0x50*/
    {"LD D,C", 1, NI},                   /*0x51*/
    {"LD D,D", 1, NI},                   /*0x52*/
    {"LD D,E", 1, NI},                   /*0x53*/
    {"LD D,H", 1, NI},                   /*0x54*/
    {"LD D,L", 1, NI},                   /*0x55*/
    {"LD D,(HL)", 1, NI},                /*0x56*/
    {"LD D,A", 1, NI},                   /*0x57*/
    {"LD E,B", 1, NI},                   /*0x58*/
    {"LD E,C", 1, NI},                   /*0x59*/
    {"LD E,D", 1, NI},                   /*0x5A*/
    {"LD E,E", 1, NI},                   /*0x5B*/
    {"LD E,H", 1, NI},                   /*0x5C*/
    {"LD E,L", 1, NI},                   /*0x5D*/
    {"LD E,(HL)", 1, NI},                /*0x5E*/
    {"LD E,A", 1, NI},                   /*0x5F*/
    {"LD H,B", 1, NI},                   /*0x60*/
    {"LD H,C", 1, NI},                   /*0x61*/
    {"LD H,D", 1, LD_H_D},               /*0x62*/
    {"LD H,E", 1, NI},                   /*0x63*/
    {"LD H,H", 1, NI},                   /*0x64*/
    {"LD H,L", 1, NI},                   /*0x65*/
    {"LD H,(HL)", 1, NI},                /*0x66*/
    {"LD H,A", 1, NI},                   /*0x67*/
    {"LD L,B", 1, NI},                   /*0x68*/
    {"LD L,C", 1, NI},                   /*0x69*/
    {"LD L,D", 1, NI},                   /*0x6A*/
    {"LD L,E", 1, LD_L_E},               /*0x6B*/
    {"LD L,H", 1, NI},                   /*0x6C*/
    {"LD L,L", 1, NI},                   /*0x6D*/
    {"LD L,(HL)", 1, NI},                /*0x6E*/
    {"LD L,A", 1, NI},                   /*0x6F*/
    {"LD (HL),B", 1, NI},                /*0x70*/
    {"LD (HL),C", 1, NI},                /*0x71*/
    {"LD (HL),D", 1, NI},                /*0x72*/
    {"LD (HL),E", 1, NI},                /*0x73*/
    {"LD (HL),H", 1, NI},                /*0x74*/
    {"LD (HL),L", 1, NI},                /*0x75*/
    {"HALT", 1, NI},                     /*0x76*/
    {"LD (HL),A", 1, NI},                /*0x77*/
    {"LD A,B", 1, NI},                   /*0x78*/
    {"LD A,C", 1, NI},                   /*0x79*/
    {"LD A,D", 1, NI},                   /*0x7A*/
    {"LD A,E", 1, NI},                   /*0x7B*/
    {"LD A,H", 1, NI},                   /*0x7C*/
    {"LD A,L", 1, NI},                   /*0x7D*/
    {"LD A,(HL)", 1, NI},                /*0x7E*/
    {"LD A,A", 1, NI},                   /*0x7F*/
    {"ADD A,B", 1, NI},                  /*0x80*/
    {"ADD A,C", 1, NI},                  /*0x81*/
    {"ADD A,D", 1, NI},                  /*0x82*/
    {"ADD A,E", 1, NI},                  /*0x83*/
    {"ADD A,H", 1, NI},                  /*0x84*/
    {"ADD A,L", 1, NI},                  /*0x85*/
    {"ADD (HL)", 1, NI},                 /*0x86*/
    {"ADD A,A", 1, NI},                  /*0x87*/
    {"ADC A,B", 1, NI},                  /*0x88*/
    {"ADC A,C", 1, NI},                  /*0x89*/
    {"ADC A,D", 1, NI},                  /*0x8A*/
    {"ADC A,E", 1, NI},                  /*0x8B*/
    {"ADC A,H", 1, NI},                  /*0x8C*/
    {"ADC A,L", 1, NI},                  /*0x8D*/
    {"ADC A,(HL)", 1, NI},               /*0x8E*/
    {"ADC A,A", 1, NI},                  /*0x8F*/
    {"SUB B", 1, NI},                    /*0x90*/
    {"SUB C", 1, NI},                    /*0x91*/
    {"SUB D", 1, NI},                    /*0x92*/
    {"SUB E", 1, NI},                    /*0x93*/
    {"SUB H", 1, NI},                    /*0x94*/
    {"SUB L", 1, NI},                    /*0x95*/
    {"SUB (HL)", 1, NI},                 /*0x96*/
    {"SUB A", 1, NI},                    /*0x97*/
    {"SBC A,B", 1, NI},                  /*0x98*/
    {"SBC A,C", 1, NI},                  /*0x99*/
    {"SBC A,D", 1, NI},                  /*0x9A*/
    {"SBC A,E", 1, NI},                  /*0x9B*/
    {"SBC A,H", 1, NI},                  /*0x9C*/
    {"SBC A,L", 1, NI},                  /*0x9D*/
    {"SBC A,(HL)", 1, NI},               /*0x9E*/
    {"SBC A,A", 1, NI},                  /*0x9F*/
    {"AND B", 1, NI},                    /*0xA0*/
    {"AND C", 1, NI},                    /*0xA1*/
    {"AND D", 1, NI},                    /*0xA2*/
    {"AND E", 1, NI},                    /*0xA3*/
    {"AND H", 1, NI},                    /*0xA4*/
    {"AND L", 1, NI},                    /*0xA5*/
    {"AND (HL)", 1, NI},                 /*0xA6*/
    {"AND A", 1, AND_A},                 /*0xA7*/
    {"XOR B", 1, NI},                    /*0xA8*/
    {"XOR C", 1, NI},                    /*0xA9*/
    {"XOR D", 1, NI},                    /*0xAA*/
    {"XOR E", 1, NI},                    /*0xAB*/
    {"XOR H", 1, NI},                    /*0xAC*/
    {"XOR L", 1, NI},                    /*0xAD*/
    {"XOR (HL)", 1, NI},                 /*0xAE*/
    {"XOR A", 1, XOR_A},                 /*0xAF*/
    {"OR B", 1, NI},                     /*0xB0*/
    {"OR C", 1, NI},                     /*0xB1*/
    {"OR D", 1, NI},                     /*0xB2*/
    {"OR E", 1, NI},                     /*0xB3*/
    {"OR H", 1, NI},                     /*0xB4*/
    {"OR L", 1, NI},                     /*0xB5*/
    {"OR(HL)", 1, NI},                   /*0xB6*/
    {"OR A", 1, NI},                     /*0xB7*/
    {"CP B", 1, NI},                     /*0xB8*/
    {"CP C", 1, NI},                     /*0xB9*/
    {"CP D", 1, NI},                     /*0xBA*/
    {"CP E", 1, NI},                     /*0xBB*/
    {"CP H", 1, CP_H},                   /*0xBC*/
    {"CP L", 1, NI},                     /*0xBD*/
    {"CP (HL)", 1, NI},                  /*0xBE*/
    {"CP A", 1, NI},                     /*0xBF*/
    {"RET NZ", 1, NI},                   /*0xC0*/
    {"POP BC", 1, NI},                   /*0xC1*/
    {"JP NZ, %X", 1, NI},                /*0xC2*/
    {"JP %X", 3, JP_16},                 /*0xC3*/
    {"CALLNZ %X", 1, NI},                /*0xC4*/
    {"PUSH BC", 1, NI},                  /*0xC5*/
    {"ADD A,%X", 1, NI},                 /*0xC6*/
    {"RST 10H", 0, NI},                  /*0xC7*/
    {"RET Z", 1, NI},                    /*0xC8*/
    {"RET", 1, NI},                      /*0xC9*/
    {"JP Z,%X", 1, NI},                  /*0xCA*/
    {"Multibyte CB", 1, NI},             /*0xCB*/
    {"CALL Z,%X", 1, NI},                /*0xCC*/
    {"CALL %X", 1, NI},                  /*0xCD*/
    {"ADC A,%X", 1, NI},                 /*0xCE*/
    {"RST 18H", 0, NI},                  /*0xCF*/
    {"RET NC", 1, NI},                   /*0xD0*/
    {"POP DE", 1, NI},                   /*0xD1*/
    {"JP NC,%X", 1, NI},                 /*0xD2*/
    {"OUT (+%X),A", 2, OUTA},            /*0xD3*/
    {"CALL NC,%X", 1, NI},               /*0xD4*/
    {"PUSH DE", 1, NI},                  /*0xD5*/
    {"SUB %X", 1, NI},                   /*0xD6*/
    {"RST", 1, NI},                      /*0xD7*/
    {"RET C", 1, NI},                    /*0xD8*/
    {"EXX", 1, EXX},                     /*0xD9*/
    {"JP C,%X", 1, NI},                  /*0xDA*/
    {"IN A,(C)", 1, NI},                 /*0xDB*/
    {"CALL C,%X", 1, NI},                /*0xDC*/
    {"Multi-byte DD", 1, NI},            /*0xDD*/
    {"SBC A,%X", 1, NI},                 /*0xDE*/
    {"RST ", 1, NI},                     /*0xDF*/
    {"RET PO,%X", 1, NI},                /*0xE0*/
    {"POP HL", 1, NI},                   /*0xE1*/
    {"JP PO,%X ", 1, NI},                /*0xE2*/
    {"EX (SP),HL", 1, NI},               /*0xE3*/
    {"CALL PO,%X", 1, NI},               /*0xE4*/
    {"PUSH HL", 1, NI},                  /*0xE5*/
    {"AND %X", 1, NI},                   /*0xE6*/
    {"RST 21H", 0, NI},                  /*0xE7*/
    {"RET PE,%X", 1, NI},                /*0xE8*/
    {"JP (HL)", 1, NI},                  /*0xE9*/
    {"JP PE,%X", 1, NI},                 /*0xEA*/
    {"EX DE,HL", 1, NI},                 /*0xEB*/
    {"CALL PE,%X", 1, NI},               /*0xEC*/
    {"Multi-byte ED", 1, NI},            /*0xED*/
    {"XOR %X", 1, NI},                   /*0xEE*/
    {"RST 28H", 1, NI},                  /*0xEF*/
    {"RET P", 1, NI},                    /*0xF0*/
    {"POP AF", 1, NI},                   /*0xF1*/
    {"JP P,%X", 1, NI},                  /*0xF2*/
    {"DI", 1, DI},                       /*0xF3*/
    {"CALL P,%X", 1, NI},                /*0xF4*/
    {"PUSH AF", 1, NI},                  /*0xF5*/
    {"OR %X", 1, NI},                    /*0xF6*/
    {"RST 31H", 0, NI},                  /*0xF7*/
    {"RET M,%X", 1, NI},                 /*0xF8*/
    {"LD SP,HL", 1, NI},                 /*0xF9*/
    {"JP M,%X", 1, NI},                  /*0xFA*/
    {"EI", 1, NI},                       /*0xFB*/
    {"CALL M,%X", 1, NI},                /*0xFC*/
    {"Multi-byte FD", 1, NI},            /*0xFD*/
    {"CP, %X", 1, NI},                   /*0xFE*/
    {"RST 38H", 1, NI}                   /*0xFF*/
};

z80_instruction ed_multi_byte_instruction_lookup[] = {
    {"", 1, NI},                  /*0x00*/
    {"", 1, NI},                  /*0x01*/
    {"", 1, NI},                  /*0x02*/
    {"", 1, NI},                  /*0x03*/
    {"", 1, NI},                  /*0x04*/
    {"", 1, NI},                  /*0x05*/
    {"", 1, NI},                  /*0x06*/
    {"", 1, NI},                  /*0x07*/
    {"", 1, NI},                  /*0x08*/
    {"", 1, NI},                  /*0x09*/
    {"", 1, NI},                  /*0x0a*/
    {"", 1, NI},                  /*0x0b*/
    {"", 1, NI},                  /*0x0c*/
    {"", 1, NI},                  /*0x0d*/
    {"", 1, NI},                  /*0x0e*/
    {"", 1, NI},                  /*0x0f*/
    {"", 1, NI},                  /*0x10*/
    {"", 1, NI},                  /*0x11*/
    {"", 1, NI},                  /*0x12*/
    {"", 1, NI},                  /*0x13*/
    {"", 1, NI},                  /*0x14*/
    {"", 1, NI},                  /*0x15*/
    {"", 1, NI},                  /*0x16*/
    {"", 1, NI},                  /*0x17*/
    {"", 1, NI},                  /*0x18*/
    {"", 1, NI},                  /*0x19*/
    {"", 1, NI},                  /*0x1a*/
    {"", 1, NI},                  /*0x1b*/
    {"", 1, NI},                  /*0x1c*/
    {"", 1, NI},                  /*0x1d*/
    {"", 1, NI},                  /*0x1e*/
    {"", 1, NI},                  /*0x1f*/
    {"", 1, NI},                  /*0x20*/
    {"", 1, NI},                  /*0x21*/
    {"", 1, NI},                  /*0x22*/
    {"", 1, NI},                  /*0x23*/
    {"", 1, NI},                  /*0x24*/
    {"", 1, NI},                  /*0x25*/
    {"", 1, NI},                  /*0x26*/
    {"", 1, NI},                  /*0x27*/
    {"", 1, NI},                  /*0x28*/
    {"", 1, NI},                  /*0x29*/
    {"", 1, NI},                  /*0x2a*/
    {"", 1, NI},                  /*0x2b*/
    {"", 1, NI},                  /*0x2c*/
    {"", 1, NI},                  /*0x2d*/
    {"", 1, NI},                  /*0x2e*/
    {"", 1, NI},                  /*0x2f*/
    {"", 1, NI},                  /*0x30*/
    {"", 1, NI},                  /*0x31*/
    {"", 1, NI},                  /*0x32*/
    {"", 1, NI},                  /*0x33*/
    {"", 1, NI},                  /*0x34*/
    {"", 1, NI},                  /*0x35*/
    {"", 1, NI},                  /*0x36*/
    {"", 1, NI},                  /*0x37*/
    {"", 1, NI},                  /*0x38*/
    {"", 1, NI},                  /*0x39*/
    {"", 1, NI},                  /*0x3a*/
    {"", 1, NI},                  /*0x3b*/
    {"", 1, NI},                  /*0x3c*/
    {"", 1, NI},                  /*0x3d*/
    {"", 1, NI},                  /*0x3e*/
    {"", 1, NI},                  /*0x3f*/
    {"", 1, NI},                  /*0x40*/
    {"", 1, NI},                  /*0x41*/
    {"", 1, NI},                  /*0x42*/
    {"", 1, NI},                  /*0x43*/
    {"", 1, NI},                  /*0x44*/
    {"", 1, NI},                  /*0x45*/
    {"", 1, NI},                  /*0x46*/
    {"LD I, A", 1, LD_I_A},       /*0x47*/
    {"", 1, NI},                  /*0x48*/
    {"", 1, NI},                  /*0x49*/
    {"", 1, NI},                  /*0x4a*/
    {"", 1, NI},                  /*0x4b*/
    {"", 1, NI},                  /*0x4c*/
    {"", 1, NI},                  /*0x4d*/
    {"", 1, NI},                  /*0x4e*/
    {"", 1, NI},                  /*0x4f*/
    {"", 1, NI},                  /*0x50*/
    {"", 1, NI},                  /*0x51*/
    {"SBC HL, DE", 2, SBC_HL_DE}, /*0x52*/
    {"", 1, NI},                  /*0x53*/
    {"", 1, NI},                  /*0x54*/
    {"", 1, NI},                  /*0x55*/
    {"", 1, NI},                  /*0x56*/
    {"", 1, NI},                  /*0x57*/
    {"", 1, NI},                  /*0x58*/
    {"", 1, NI},                  /*0x59*/
    {"", 1, NI},                  /*0x5a*/
    {"", 1, NI},                  /*0x5b*/
    {"", 1, NI},                  /*0x5c*/
    {"", 1, NI},                  /*0x5d*/
    {"", 1, NI},                  /*0x5e*/
    {"", 1, NI},                  /*0x5f*/
    {"", 1, NI},                  /*0x60*/
    {"", 1, NI},                  /*0x61*/
    {"", 1, NI},                  /*0x62*/
    {"", 1, NI},                  /*0x63*/
    {"", 1, NI},                  /*0x64*/
    {"", 1, NI},                  /*0x65*/
    {"", 1, NI},                  /*0x66*/
    {"", 1, NI},                  /*0x67*/
    {"", 1, NI},                  /*0x68*/
    {"", 1, NI},                  /*0x69*/
    {"", 1, NI},                  /*0x6a*/
    {"", 1, NI},                  /*0x6b*/
    {"", 1, NI},                  /*0x6c*/
    {"", 1, NI},                  /*0x6d*/
    {"", 1, NI},                  /*0x6e*/
    {"", 1, NI},                  /*0x6f*/
    {"", 1, NI},                  /*0x70*/
    {"", 1, NI},                  /*0x71*/
    {"", 1, NI},                  /*0x72*/
    {"", 1, NI},                  /*0x73*/
    {"", 1, NI},                  /*0x74*/
    {"", 1, NI},                  /*0x75*/
    {"", 1, NI},                  /*0x76*/
    {"", 1, NI},                  /*0x77*/
    {"", 1, NI},                  /*0x78*/
    {"", 1, NI},                  /*0x79*/
    {"", 1, NI},                  /*0x7a*/
    {"", 1, NI},                  /*0x7b*/
    {"", 1, NI},                  /*0x7c*/
    {"", 1, NI},                  /*0x7d*/
    {"", 1, NI},                  /*0x7e*/
    {"", 1, NI},                  /*0x7f*/
    {"", 1, NI},                  /*0x80*/
    {"", 1, NI},                  /*0x81*/
    {"", 1, NI},                  /*0x82*/
    {"", 1, NI},                  /*0x83*/
    {"", 1, NI},                  /*0x84*/
    {"", 1, NI},                  /*0x85*/
    {"", 1, NI},                  /*0x86*/
    {"", 1, NI},                  /*0x87*/
    {"", 1, NI},                  /*0x88*/
    {"", 1, NI},                  /*0x89*/
    {"", 1, NI},                  /*0x8a*/
    {"", 1, NI},                  /*0x8b*/
    {"", 1, NI},                  /*0x8c*/
    {"", 1, NI},                  /*0x8d*/
    {"", 1, NI},                  /*0x8e*/
    {"", 1, NI},                  /*0x8f*/
    {"", 1, NI},                  /*0x90*/
    {"", 1, NI},                  /*0x91*/
    {"", 1, NI},                  /*0x92*/
    {"", 1, NI},                  /*0x93*/
    {"", 1, NI},                  /*0x94*/
    {"", 1, NI},                  /*0x95*/
    {"", 1, NI},                  /*0x96*/
    {"", 1, NI},                  /*0x97*/
    {"", 1, NI},                  /*0x98*/
    {"", 1, NI},                  /*0x99*/
    {"", 1, NI},                  /*0x9a*/
    {"", 1, NI},                  /*0x9b*/
    {"", 1, NI},                  /*0x9c*/
    {"", 1, NI},                  /*0x9d*/
    {"", 1, NI},                  /*0x9e*/
    {"", 1, NI},                  /*0x9f*/
    {"", 1, NI},                  /*0xa0*/
    {"", 1, NI},                  /*0xa1*/
    {"", 1, NI},                  /*0xa2*/
    {"", 1, NI},                  /*0xa3*/
    {"", 1, NI},                  /*0xa4*/
    {"", 1, NI},                  /*0xa5*/
    {"", 1, NI},                  /*0xa6*/
    {"", 1, NI},                  /*0xa7*/
    {"", 1, NI},                  /*0xa8*/
    {"", 1, NI},                  /*0xa9*/
    {"", 1, NI},                  /*0xaa*/
    {"", 1, NI},                  /*0xab*/
    {"", 1, NI},                  /*0xac*/
    {"", 1, NI},                  /*0xad*/
    {"", 1, NI},                  /*0xae*/
    {"", 1, NI},                  /*0xaf*/
    {"", 1, NI},                  /*0xb0*/
    {"", 1, NI},                  /*0xb1*/
    {"", 1, NI},                  /*0xb2*/
    {"", 1, NI},                  /*0xb3*/
    {"", 1, NI},                  /*0xb4*/
    {"", 1, NI},                  /*0xb5*/
    {"", 1, NI},                  /*0xb6*/
    {"", 1, NI},                  /*0xb7*/
    {"", 1, NI},                  /*0xb8*/
    {"", 1, NI},                  /*0xb9*/
    {"", 1, NI},                  /*0xba*/
    {"", 1, NI},                  /*0xbb*/
    {"", 1, NI},                  /*0xbc*/
    {"", 1, NI},                  /*0xbd*/
    {"", 1, NI},                  /*0xbe*/
    {"", 1, NI},                  /*0xbf*/
    {"", 1, NI},                  /*0xc0*/
    {"", 1, NI},                  /*0xc1*/
    {"", 1, NI},                  /*0xc2*/
    {"", 1, NI},                  /*0xc3*/
    {"", 1, NI},                  /*0xc4*/
    {"", 1, NI},                  /*0xc5*/
    {"", 1, NI},                  /*0xc6*/
    {"", 1, NI},                  /*0xc7*/
    {"", 1, NI},                  /*0xc8*/
    {"", 1, NI},                  /*0xc9*/
    {"", 1, NI},                  /*0xca*/
    {"", 1, NI},                  /*0xcb*/
    {"", 1, NI},                  /*0xcc*/
    {"", 1, NI},                  /*0xcd*/
    {"", 1, NI},                  /*0xce*/
    {"", 1, NI},                  /*0xcf*/
    {"", 1, NI},                  /*0xd0*/
    {"", 1, NI},                  /*0xd1*/
    {"", 1, NI},                  /*0xd2*/
    {"", 1, NI},                  /*0xd3*/
    {"", 1, NI},                  /*0xd4*/
    {"", 1, NI},                  /*0xd5*/
    {"", 1, NI},                  /*0xd6*/
    {"", 1, NI},                  /*0xd7*/
    {"", 1, NI},                  /*0xd8*/
    {"", 1, NI},                  /*0xd9*/
    {"", 1, NI},                  /*0xda*/
    {"", 1, NI},                  /*0xdb*/
    {"", 1, NI},                  /*0xdc*/
    {"", 1, NI},                  /*0xdd*/
    {"", 1, NI},                  /*0xde*/
    {"", 1, NI},                  /*0xdf*/
    {"", 1, NI},                  /*0xe0*/
    {"", 1, NI},                  /*0xe1*/
    {"", 1, NI},                  /*0xe2*/
    {"", 1, NI},                  /*0xe3*/
    {"", 1, NI},                  /*0xe4*/
    {"", 1, NI},                  /*0xe5*/
    {"", 1, NI},                  /*0xe6*/
    {"", 1, NI},                  /*0xe7*/
    {"", 1, NI},                  /*0xe8*/
    {"", 1, NI},                  /*0xe9*/
    {"", 1, NI},                  /*0xea*/
    {"", 1, NI},                  /*0xeb*/
    {"", 1, NI},                  /*0xec*/
    {"", 1, NI},                  /*0xed*/
    {"", 1, NI},                  /*0xee*/
    {"", 1, NI},                  /*0xef*/
    {"", 1, NI},                  /*0xf0*/
    {"", 1, NI},                  /*0xf1*/
    {"", 1, NI},                  /*0xf2*/
    {"", 1, NI},                  /*0xf3*/
    {"", 1, NI},                  /*0xf4*/
    {"", 1, NI},                  /*0xf5*/
    {"", 1, NI},                  /*0xf6*/
    {"", 1, NI},                  /*0xf7*/
    {"", 1, NI},                  /*0xf8*/
    {"", 1, NI},                  /*0xf9*/
    {"", 1, NI},                  /*0xfa*/
    {"", 1, NI},                  /*0xfb*/
    {"", 1, NI},                  /*0xfc*/
    {"", 1, NI},                  /*0xfd*/
    {"", 1, NI},                  /*0xfe*/
    {"", 1, NI},                  /*0xff*/
};

z80_instruction fd_multi_byte_instruction_lookup[] = {};
z80_instruction cb_multi_byte_instruction_lookup[] = {
    {"RLC B", 1, NI},       /*0x00*/
    {"RLC C", 1, NI},       /*0x01*/
    {"RLC D", 1, NI},       /*0x02*/
    {"RLC E", 1, NI},       /*0x03*/
    {"RLC H", 1, NI},       /*0x04*/
    {"RLC L", 1, NI},       /*0x05*/
    {"RLC (HL)", 1, NI},    /*0x06*/
    {"RLC A", 1, NI},       /*0x07*/
    {"RRC B", 1, NI},       /*0x08*/
    {"RRC C", 1, NI},       /*0x09*/
    {"RRC D", 1, NI},       /*0x0A*/
    {"RRC E", 1, NI},       /*0x0B*/
    {"RRC H", 1, NI},       /*0x0C*/
    {"RRC L", 1, NI},       /*0x0D*/
    {"RRC (HL)", 1, NI},    /*0x0E*/
    {"RRC A", 1, NI},       /*0x0F*/
    {"RL B", 1, NI},        /*0x10*/
    {"RL C", 1, NI},        /*0x11*/
    {"RL D", 1, NI},        /*0x12*/
    {"RL E", 1, NI},        /*0x13*/
    {"RL H", 1, NI},        /*0x14*/
    {"RL L", 1, NI},        /*0x15*/
    {"RL (HL)", 1, NI},     /*0x16*/
    {"RL A", 1, NI},        /*0x17*/
    {"RR B", 1, NI},        /*0x18*/
    {"RR C", 1, NI},        /*0x19*/
    {"RR D", 1, NI},        /*0x1A*/
    {"RR E", 1, NI},        /*0x1B*/
    {"RR H", 1, NI},        /*0x1C*/
    {"RR L", 1, NI},        /*0x1D*/
    {"RR (HL)", 1, NI},     /*0x1E*/
    {"RR A", 1, NI},        /*0x1F*/
    {"SLA B", 1, NI},       /*0x20*/
    {"SLA C", 1, NI},       /*0x21*/
    {"SLA D", 1, NI},       /*0x22*/
    {"SLA E", 1, NI},       /*0x23*/
    {"SLA H", 1, NI},       /*0x24*/
    {"SLA L", 1, NI},       /*0x25*/
    {"SLA (HL)", 1, NI},    /*0x26*/
    {"SLA A", 1, NI},       /*0x27*/
    {"SRA B", 1, NI},       /*0x28*/
    {"SRA B", 1, NI},       /*0x29*/
    {"SRA D", 1, NI},       /*0x2A*/
    {"SRA E", 1, NI},       /*0x2B*/
    {"SRA H", 1, NI},       /*0x2C*/
    {"SRA L", 1, NI},       /*0x2D*/
    {"SRA (HL)", 1, NI},    /*0x2E*/
    {"unused", 1, NI},      /*0x2F*/
    {"unused", 1, NI},      /*0x30*/
    {"unused", 1, NI},      /*0x31*/
    {"unused", 1, NI},      /*0x32*/
    {"unused", 1, NI},      /*0x33*/
    {"unused", 1, NI},      /*0x34*/
    {"unused", 1, NI},      /*0x35*/
    {"unused", 1, NI},      /*0x36*/
    {"unused", 1, NI},      /*0x37*/
    {"SRL B", 1, NI},       /*0x38*/
    {"SRL C", 1, NI},       /*0x39*/
    {"SRL D", 1, NI},       /*0x3A*/
    {"SRL E", 1, NI},       /*0x3B*/
    {"SRL H", 1, NI},       /*0x3C*/
    {"SRL L", 1, NI},       /*0x3D*/
    {"SRL (HL)", 1, NI},    /*0x3E*/
    {"SRL A", 1, NI},       /*0x3F*/
    {"BIT 0", 1, NI},       /*0x40*/
    {"BIT 0", 1, NI},       /*0x41*/
    {"BIT 0", 1, NI},       /*0x42*/
    {"BIT 0", 1, NI},       /*0x43*/
    {"BIT 0", 1, NI},       /*0x44*/
    {"BIT 0", 1, NI},       /*0x45*/
    {"BIT 0", 1, NI},       /*0x46*/
    {"BIT 0", 1, NI},       /*0x47*/
    {"BIT 1", 1, NI},       /*0x48*/
    {"BIT 1", 1, NI},       /*0x49*/
    {"BIT 1", 1, NI},       /*0x4A*/
    {"BIT 1", 1, NI},       /*0x4B*/
    {"BIT 1", 1, NI},       /*0x4C*/
    {"BIT 1", 1, NI},       /*0x4D*/
    {"BIT 1", 1, NI},       /*0x4E*/
    {"BIT 1", 1, NI},       /*0x4F*/
    {"BIT 2", 1, NI},       /*0x50*/
    {"BIT 2", 1, NI},       /*0x51*/
    {"BIT 2", 1, NI},       /*0x52*/
    {"BIT 2", 1, NI},       /*0x53*/
    {"BIT 2", 1, NI},       /*0x54*/
    {"BIT 2", 1, NI},       /*0x55*/
    {"BIT 2", 1, NI},       /*0x56*/
    {"BIT 2", 1, NI},       /*0x57*/
    {"BIT 3", 1, NI},       /*0x58*/
    {"BIT 3", 1, NI},       /*0x59*/
    {"BIT 3,D", 1, NI},     /*0x5A*/
    {"BIT 3", 1, NI},       /*0x5B*/
    {"BIT 3", 1, NI},       /*0x5C*/
    {"BIT 3", 1, NI},       /*0x5D*/
    {"BIT 3", 1, NI},       /*0x5E*/
    {"BIT 3", 1, NI},       /*0x5F*/
    {"BIT 4", 1, NI},       /*0x60*/
    {"BIT 4", 1, NI},       /*0x61*/
    {"BIT 4", 1, NI},       /*0x62*/
    {"BIT 4", 1, NI},       /*0x63*/
    {"BIT 4", 1, NI},       /*0x64*/
    {"BIT 4", 1, NI},       /*0x65*/
    {"BIT 4", 1, NI},       /*0x66*/
    {"BIT 4", 1, NI},       /*0x67*/
    {"BIT 5", 1, NI},       /*0x68*/
    {"BIT 5", 1, NI},       /*0x69*/
    {"BIT 5", 1, NI},       /*0x6A*/
    {"BIT 5", 1, NI},       /*0x6B*/
    {"BIT 5", 1, NI},       /*0x6C*/
    {"BIT 5", 1, NI},       /*0x6D*/
    {"BIT 5, (HL)", 1, NI}, /*0x6E*/
    {"BIT 5", 1, NI},       /*0x6F*/
    {"BIT 6, (HL)", 1, NI}, /*0x70*/
    {"BIT 6, (HL)", 1, NI}, /*0x71*/
    {"BIT 6, (HL)", 1, NI}, /*0x72*/
    {"BIT 6, (HL)", 1, NI}, /*0x73*/
    {"BIT 6, (HL)", 1, NI}, /*0x74*/
    {"BIT 6, (HL)", 1, NI}, /*0x75*/
    {"BIT 6, (HL)", 1, NI}, /*0x76*/
    {"BIT 6, (HL)", 1, NI}, /*0x77*/
    {"BIT 7, (HL)", 1, NI}, /*0x78*/
    {"BIT 7, (HL)", 1, NI}, /*0x79*/
    {"BIT 7, (HL)", 1, NI}, /*0x7A*/
    {"BIT 7, (HL)", 1, NI}, /*0x7B*/
    {"BIT 7, (HL)", 1, NI}, /*0x7C*/
    {"BIT 7, (HL)", 1, NI}, /*0x7D*/
    {"BIT 7, (HL)", 1, NI}, /*0x7E*/
    {"BIT 7, (HL)", 1, NI}, /*0x7F*/
    {"RES 0, (HL)", 1, NI}, /*0x80*/
    {"RES 0, (HL)", 1, NI}, /*0x81*/
    {"RES 0, (HL)", 1, NI}, /*0x82*/
    {"RES 0, (HL)", 1, NI}, /*0x83*/
    {"RES 0, (HL)", 1, NI}, /*0x84*/
    {"RES 0, (HL)", 1, NI}, /*0x85*/
    {"RES 0, (HL)", 1, NI}, /*0x86*/
    {"RES 0, (HL)", 1, NI}, /*0x87*/
    {"RES 1, (HL)", 1, NI}, /*0x88*/
    {"RES 1, (HL)", 1, NI}, /*0x89*/
    {"RES 1, (HL)", 1, NI}, /*0x8A*/
    {"RES 1, (HL)", 1, NI}, /*0x8B*/
    {"RES 1, (HL)", 1, NI}, /*0x8C*/
    {"RES 1, (HL)", 1, NI}, /*0x8D*/
    {"RES 1, (HL)", 1, NI}, /*0x8E*/
    {"RES 1, (HL)", 1, NI}, /*0x8F*/
    {"RES 2, (HL)", 1, NI}, /*0x90*/
    {"RES 2, (HL)", 1, NI}, /*0x91*/
    {"RES 2, (HL)", 1, NI}, /*0x92*/
    {"RES 2, (HL)", 1, NI}, /*0x93*/
    {"RES 2, (HL)", 1, NI}, /*0x94*/
    {"RES 2, (HL)", 1, NI}, /*0x95*/
    {"RES 2, (HL)", 1, NI}, /*0x96*/
    {"RES 2, (HL)", 1, NI}, /*0x97*/
    {"RES 3, (HL)", 1, NI}, /*0x98*/
    {"RES 3, (HL)", 1, NI}, /*0x99*/
    {"RES 3, (HL)", 1, NI}, /*0x9A*/
    {"RES 3, (HL)", 1, NI}, /*0x9B*/
    {"RES 3, (HL)", 1, NI}, /*0x9C*/
    {"RES 3, (HL)", 1, NI}, /*0x9D*/
    {"RES 3, (HL)", 1, NI}, /*0x9E*/
    {"RES 3, (HL)", 1, NI}, /*0x9F*/
    {"RES 4, (HL)", 1, NI}, /*0xA0*/
    {"RES 4, (HL)", 1, NI}, /*0xA1*/
    {"RES 4, (HL)", 1, NI}, /*0xA2*/
    {"RES 4, (HL)", 1, NI}, /*0xA3*/
    {"RES 4, (HL)", 1, NI}, /*0xA4*/
    {"RES 4, (HL)", 1, NI}, /*0xA5*/
    {"RES 4, (HL)", 1, NI}, /*0xA6*/
    {"RES 4, (HL)", 1, NI}, /*0xA7*/
    {"RES 5, (HL)", 1, NI}, /*0xA8*/
    {"RES 5, (HL)", 1, NI}, /*0xA9*/
    {"RES 5, (HL)", 1, NI}, /*0xAA*/
    {"RES 5, (HL)", 1, NI}, /*0xAB*/
    {"RES 5, (HL)", 1, NI}, /*0xAC*/
    {"RES 5, (HL)", 1, NI}, /*0xAD*/
    {"RES 5, (HL)", 1, NI}, /*0xAE*/
    {"RES 5, (HL)", 1, NI}, /*0xAF*/
    {"RES 6, (HL)", 1, NI}, /*0xB0*/
    {"RES 6, (HL)", 1, NI}, /*0xB1*/
    {"RES 6, (HL)", 1, NI}, /*0xB2*/
    {"RES 6, (HL)", 1, NI}, /*0xB3*/
    {"RES 6, (HL)", 1, NI}, /*0xB4*/
    {"RES 6, (HL)", 1, NI}, /*0xB5*/
    {"RES 6, (HL)", 1, NI}, /*0xB6*/
    {"RES 6, (HL)", 1, NI}, /*0xB7*/
    {"RES 7, (HL)", 1, NI}, /*0xB8*/
    {"RES 7, (HL)", 1, NI}, /*0xB9*/
    {"RES 7, (HL)", 1, NI}, /*0xBA*/
    {"RES 7, (HL)", 1, NI}, /*0xBB*/
    {"RES 7, (HL)", 1, NI}, /*0xBC*/
    {"RES 7, (HL)", 1, NI}, /*0xBD*/
    {"RES 7, H", 1, NI},    /*0xBE*/
    {"RES 7, (HL)", 1, NI}, /*0xBF*/
    {"SET 0, (HL)", 1, NI}, /*0xC0*/
    {"SET 0, (HL)", 1, NI}, /*0xC1*/
    {"SET 0, (HL)", 1, NI}, /*0xC2*/
    {"SET 0, (HL)", 1, NI}, /*0xC3*/
    {"SET 0, (HL)", 1, NI}, /*0xC4*/
    {"SET 0, (HL)", 1, NI}, /*0xC5*/
    {"SET 0, (HL)", 1, NI}, /*0xC6*/
    {"SET 0, (HL)", 1, NI}, /*0xC7*/
    {"SET 1, (HL)", 1, NI}, /*0xC8*/
    {"SET 1, (HL)", 1, NI}, /*0xC9*/
    {"SET 1, (HL)", 1, NI}, /*0xCA*/
    {"SET 1, (HL)", 1, NI}, /*0xCB*/
    {"SET 1, (HL)", 1, NI}, /*0xCC*/
    {"SET 1, (HL)", 1, NI}, /*0xCD*/
    {"SET 1, (HL)", 1, NI}, /*0xCE*/
    {"SET 1, (HL)", 1, NI}, /*0xCF*/
    {"SET 2, (HL)", 1, NI}, /*0xD0*/
    {"SET 2, (HL)", 1, NI}, /*0xD1*/
    {"SET 2, (HL)", 1, NI}, /*0xD2*/
    {"SET 2, (HL)", 1, NI}, /*0xD3*/
    {"SET 2, (HL)", 1, NI}, /*0xD4*/
    {"SET 2, (HL)", 1, NI}, /*0xD5*/
    {"SET 2, (HL)", 1, NI}, /*0xD6*/
    {"SET 2, (HL)", 1, NI}, /*0xD7*/
    {"SET 3, (HL)", 1, NI}, /*0xD8*/
    {"SET 3, (HL)", 1, NI}, /*0xD9*/
    {"SET 3, (HL)", 1, NI}, /*0xDA*/
    {"SET 3, (HL)", 1, NI}, /*0xDB*/
    {"SET 3, (HL)", 1, NI}, /*0xDC*/
    {"SET 3, (HL)", 1, NI}, /*0xDD*/
    {"SET 3, (HL)", 1, NI}, /*0xDE*/
    {"SET 3, (HL)", 1, NI}, /*0xDF*/
    {"SET 4, (HL)", 1, NI}, /*0xE0*/
    {"SET 4, (HL)", 1, NI}, /*0xE1*/
    {"SET 4, (HL)", 1, NI}, /*0xE2*/
    {"SET 4, (HL)", 1, NI}, /*0xE3*/
    {"SET 4, (HL)", 1, NI}, /*0xE4*/
    {"SET 4, (HL)", 1, NI}, /*0xE5*/
    {"SET 4, (HL)", 1, NI}, /*0xE6*/
    {"SET 4, (HL)", 1, NI}, /*0xE7*/
    {"SET 5, (HL)", 1, NI}, /*0xE8*/
    {"SET 5, (HL)", 1, NI}, /*0xE9*/
    {"SET 5, (HL)", 1, NI}, /*0xEA*/
    {"SET 5, (HL)", 1, NI}, /*0xEB*/
    {"SET 5, (HL)", 1, NI}, /*0xEC*/
    {"SET 5, (HL)", 1, NI}, /*0xED*/
    {"SET 5, (HL)", 1, NI}, /*0xEE*/
    {"SET 5, (HL)", 1, NI}, /*0xEF*/
    {"SET 6, (HL)", 1, NI}, /*0xF0*/
    {"SET 6, (HL)", 1, NI}, /*0xF1*/
    {"SET 6, (HL)", 1, NI}, /*0xF2*/
    {"SET 6, (HL)", 1, NI}, /*0xF3*/
    {"SET 6, (HL)", 1, NI}, /*0xF4*/
    {"SET 6, (HL)", 1, NI}, /*0xF5*/
    {"SET 6, (HL)", 1, NI}, /*0xF6*/
    {"SET 6, (HL)", 1, NI}, /*0xF7*/
    {"SET 7, (HL)", 1, NI}, /*0xF8*/
    {"SET 7, (HL)", 1, NI}, /*0xF9*/
    {"SET 7, (HL)", 1, NI}, /*0xFA*/
    {"SET 7, (HL)", 1, NI}, /*0xFB*/
    {"SET 7, (HL)", 1, NI}, /*0xFC*/
    {"SET 7, (HL)", 1, NI}, /*0xFD*/
    {"SET 7, (HL)", 1, NI}, /*0xFE*/
    {"SET 7, L", 1, NI},    /*0xFF*/
};

z80_instruction dd_multi_byte_instruction_lookup[] = {};
