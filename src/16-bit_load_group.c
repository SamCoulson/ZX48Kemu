#include "../include/16-bit_load_group.h"
#include <stdio.h> // Debug output
// LOAD data methods for 16-bit groups

// *** 16-BIT LOAD GROUP ***

// LD dd,nn
// Copy 2-byte integer value into 16-bit register 
// OpCodes: 0x01, 0x11, 0x21, 0x31

// LD IX,nn
// Copy 2-byte integer value into IX 16-bit register
// OpCodes: 0xDD21

// LD IY,nn
// Copy 2-byte integer value into IY 16-bit register
// OpCodes: 0xFD21

// LD HL,(nn)
// Copy value into 16-bit HL register at the 16-bit integer address 
// OpCodes: 0x2A

// LD dd,(nn)
// Copy value at 16-bit integer address to 16-bit register
// OpCodes: 0xED4B 

// LD IX,(nn)
// Copy value into 16-bit HL IX register at the 16-bit integer address
// OpCodes: 0xDD2A

// LD IY,(nn)
// Copy value into 16-bit HL IY register at the 16-bit integer address
// OpCodes: 0xFD2A

// LD(nn),HL
// Copy 16-bit HL register value into 16-bit integer address 
// OpCodes: 0x22

// LD(nn),dd
// Copy 16-bit register value into 16-bit integer address
// OpCodes: 0xED43, 0xED53

// LD(nn),IX
// Copy 16-bit IX register value into 16-bit integer address
// OpCodes: 0xDD22

// LD(nn),IY
// Copy 16-bit IY register value into 16-bit integer address
// OpCodes: 0xFD22

// LD SP,HL
// Copy 16-bit HL register value into SP 16-bit register
// OpCodes: 0xF9

// LD SP,IX
// Copy 16-bit IX register value into SP 16-bit register
// OpCodes: 0xDDF9

// LD SP,IY
// Copy 16-bit IY register value into SP 16-bit register
// OpCodes: 0xFDF9

void LD16( uint16_t* dst, uint16_t* src){
	*dst = *src;			
}

// PUSH qq
// Copy 16-bit register value onto the top of the stack
// OpCodes: 0xC5, 0xD5, 0xE5 0xF5

// PUSH IX
// Copy 16-bit IX register value onto the top of the stack
// OpCodes: 0xDDE5

// PUSH IY
// Copy 16-bit IY register value onto the top of the stack
// OpCodes: 0xFDE5

void PUSH( uint16_t* spAddrs, uint16_t* sp, uint16_t* srcVal ){
	
	// Decrement the stack pointer
	--*sp;

	// Write the value into the address specified by SP
	*(spAddrs-1) = *srcVal;

	// Decrement stack pointer again
	--*sp;	
}

// POP qq
// Copy 2-bytes from top of stack in a 16-bit register
// OpCodes: 0xC1, 0xD1, 0xE1 0xF1

// POP IX
// Copy 2-bytes from top of stack in a 16-bit IX register
// OpCodes: 0xDDE1

// POP IY
// Copy 2-bytes from top of stack in a 16-bit IY register
// OpCodes: 0xFDE1

void POP( uint16_t* spAddrs, uint16_t* sp, uint16_t* dstVal ){
	
	// Decrement the stack pointer
	++*sp;

	// Write the value into the address specified by dst value either a register or ix or iy
	*dstVal = *spAddrs;

	// Decrement stack pointer again
	++*sp;
}	
