#ifndef INC_CPU_H
#define INC_CPU_H

#include <stdint.h>
#include <stdio.h>
#include "memory.h"

// Start the CPU running
void start( uint16_t addrs );

// Set program counter to a pointer to a memory address
void setPC( uint16_t mem );

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

// 8-Bit Z80 alternative general purpose registers
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

// 16-Bit index registers
uint16_t ix;
uint16_t iy;

// 16-Bit stack pointer and program counter 
uint16_t sp;
uint16_t pc; // AKA Instruction pointer

// IFFI (Interrupt enabled flip-flop)
int iff1;
int iff2;

// Structure to hold pointers to all register union
typedef struct _REGISTERS{

	// Pointers to main registers
	uint8_t* const a;
	uint8_t* const f; 
	uint16_t* const af;

	uint8_t* b;
	uint8_t* c;
	uint16_t* bc;

	uint8_t* d;
	uint8_t* e;
	uint16_t* de;

	uint8_t* h;
	uint8_t* l;
	uint16_t* hl;
	
	// Pointers alternative general purpose registers
	uint8_t* alta;
	uint8_t* altf; 
	uint16_t* altaf;

	uint8_t* altb;
	uint8_t* altc;
	uint16_t* altbc;

	uint8_t* altd;
	uint8_t* alte;
	uint16_t* altde;

	uint8_t* alth;
	uint8_t* altl;
	uint16_t* althl;	
	
	// 8-Bit special registers - Interrupt vector
	uint8_t* i;
	uint8_t* r;
	uint16_t* ir;

	// 16-Bit index registers
	uint16_t* ix;
	uint16_t* iy;

	// 16-Bit stack pointer and program counter 
	uint16_t* sp;
	uint16_t* pc; // AKA Instruction pointer

	// IFFI (Interrupt enabled flip-flop)
	int* const iff1;
	int* const iff2;

}Registers;

//static struct _REGISTERS *reg = &registers;


int initCPU();

// Execute instructions
void execute(uint8_t* opcode);

// Utility functions

// Memory handling

// Read byte from memory given 16-bit address
uint8_t readByte( uint16_t romAddrs );

// Write byte to memory given 16-bit address
void writeByte( uint16_t addrs, uint8_t val );

// But first byte in to HO byte, and second byte in to LO byte of a word
uint16_t byteToWord(uint8_t *byte1, uint8_t *byte2);

// Return the LO from a word
uint8_t getLOByte(uint16_t *word);

// Return the Ho from a word
uint8_t getHOByte(uint16_t *word);

#endif // INC_CPU_H
