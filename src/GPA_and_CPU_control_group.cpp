#include "../include/cpu.h"

// ***General-Purpose Arithmetic and CPU Control Group***
	
// DAA
// Conditionally adjust the accumulator for BCD conditions
// OpCodes: 0x27

// CPL
// Contents of accumulator A are inverted to the complement
// OpCodes: 0x2F 

// NEG
// The contentsof the accumulator is negated (subtracted from 0)
// OpCodes: 0xED44

// CCF
// The carry flag in the F register is negated
// OpCodes: 0x3F

// SCF
// The carry flag in the F register is set
// OpCodes: 0x37

// NOP
// Perform no operation during the machine cycle
// OpCodes: 0x00
void CPU::NOP(){
	// Suposed to wait 6 T-cycles
}

// HALT
// Suspend CPU operation until an interupt is received
// OpCodes: 0x76

// DI
// Disable the maskable interrupts IFF1 and IFF2 by resetting enable interupt flip-flop
// OpCodes: 0xF3
void CPU::DI(){
	// Disable maskable interrupt so that maskable interrupt requests
	// are ignored` 
	IFF1 = false;
	IFF2 = false;	
}

// EI
// Enable interupt instruction sets, sets both IFF1 and IFF2.
// OpCodes: 0xFB
 
// IM 0
// Set interrupt mode 0
// OpCodes: 0xED46

// IM 1
// Set interrupt mode 1
// opCodes: 0xED56

// IM 2
// Set interrupt mode 2
// OpCodes: 0xED5E

