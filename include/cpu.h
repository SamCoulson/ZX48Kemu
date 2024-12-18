#ifndef INC_CPU_H
#define INC_CPU_H

#include <stdint.h>
#include <stdio.h>
//#include "memory.h"

#define READ 0
#define WRITE 1
extern int t_counter;
extern union _AF af;
extern union _BC bc;
extern union _DE de;
extern union _HL hl;
extern union _ALTAF altaf;
extern union _ALTBC altbc;
extern union _ALTDE altde;
extern union _ALTHL althl;

extern union _IR ir;

// 16-Bit index registers
extern uint16_t ix;
extern uint16_t iy;

// 16-Bit stack pointer and program counter 
extern uint16_t sp;
extern uint16_t pc; // AKA Instruction pointer

// IFFI (Interrupt enabled flip-flop)
extern int iff1;
extern int iff2;

// Mode ( CPU mode )
extern int mode;

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

	// CPU Mode
	int* const mode;

}Registers;

extern Registers* reg;

//uint8_t ports[256];

// Start the CPU running
extern void run( uint16_t addrs );

int initCPU();

// Execute instructions
void execute(uint8_t* opcode);

// Utility functions

// Port handing
void mapPort( unsigned int port, uint8_t(*func)(int, uint8_t) );

// Memory handling

// Get next byte along in memory, will progress the PC by 1 
uint8_t* getNextByte();

// Get next word from next two locations in memory, will progress the PC by 2
uint16_t* getNextWord();

// Read the next byte along, does not progress pc
uint8_t readNextByte();

// read next word along, does not progress pc
uint16_t readNextWord();

// Get the byte specified memory location
uint8_t* getByteAt( uint16_t addrs );

// Get the word at the specified location
uint16_t* getWordAt( uint16_t *addrs );

// Read word at memory location
uint16_t readWordAt( uint16_t *addrs );

// Read byte at memory location
uint16_t readByteAt( uint16_t addrs );

// Write byte to memory given 16-bit address
void writeByte( uint16_t addrs, uint8_t val );

// But first byte in to HO byte, and second byte in to LO byte of a word
uint16_t byteToWord(uint8_t *byte1, uint8_t *byte2);

// Return the LO from a word
uint8_t getLOByte(uint16_t *word);

// Return the Ho from a word
uint8_t getHOByte(uint16_t *word);

uint8_t readPort(uint16_t portAddrs);

#endif // INC_CPU_H
