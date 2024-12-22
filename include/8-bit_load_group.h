#ifndef INC_8BITLOADGROUP_H
#define INC_8BITLOADGROUP_H 

#include <stdint.h>
#include "../include/util_bit_operations.h"

// LOAD data methods for 8-bit groups

// *** 8-BIT LOAD GROUP ***
//
// LD r,r'
// Copy 8-bit register value to 8-bit register
// OpCodes: 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4F, 
//	    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5F, 
//	    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6F, 
// 						      0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7F 	

// LD r,n
// Copy 8-bit constant value to 8-bit register
// OpCodes: 0x0E, 0x1E, 0x2E, 0x3E, 0x06, 0x16, 0x26

// LD r,(HL)
// Copy value from address in HL to 8-bit register
// OpCodes: 0x4E, 0x5E, 0x6E, 0x7E, 0x46, 0x56, 0x66

// LD r,(IX+d)
// Copy value from address in IX+d to 8-bit register
// OpCodes: 0xDD46, 0xDD4E, 0xDD56, 0xDD5E, 0xDD66, 0xDD6E, 0xDD7E

// LD r,(IY+d)
// Copy value from address in IY+d HL to 8-bit register
// OpCodes: 0xFD46, 0xFD4E, 0xFD56, 0xFD5E, 0xFD66, 0xFD6E, 0xFD7E

// LD (HL), r
// Copy value of 8-bit register to address in HL
// OpCodes: 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x77


// LD (IY+d),r
// Copy value from 8-bit register to address in IY+d
// OpCodes: 0xFD70, 0xFD71, 0xFD72, 0xFD73, 0xFD74, 0xFD75, 0xFD77

// LD (IX+d),r
// Copy value from 8-bit register to address in IX+d 
// OpCodes: 0xDD70, 0xDD71, 0xDD72, 0xDD73, 0xDD74, 0xDD75, 0xDD77

// LD (HL),n
// Copy integer value in 8-bit register into address at HL 
// OpCodes: 0x36

// LD (IX+d),n
// Copy integer value in 8-bit register to address in IX+d
// OpCodes: 0xDD36

// LD (IY+d),n
// Copy integer value in 8-bit register to address in IY+d 
// OpCodes: 0xFD36

// LD A,(BC)
// Copy value in address of BC into 8-bit register A
// OpCodes: 0x0A

// LD A,(DE)
// Copy value in address of BC into 8-bit register A
// OpCodes: 0x1A

// LD A,(nn)
// Copy value in 16-bit integer address to 8-bit register A **Write to PORT**
// OpCodes: 0x3A

// LD (BC),A
// Copy value in 8-bit register A to address in BC
// OpCodes: 0x02

// LD (DE),A
// Copy value in 8-bit register A to address in DE
// OpCodes: 0x12

// LD (nn),A
// Copy 16-bit integer value at 16-bit register address to 8-bit register 
// OpCodes: 0x32

// LD I,A
// Copy value in 8-bit register I to 8-bit register A
// OpCodes: 0xED47

// LD R,A
// Copy value in 8-bit register R to 8-bit register A
// OpCodes: 0xED4F

void LD( uint8_t* dst, uint8_t* src );

// LD A,I 
// Copy value in 8-bit register I to 8-bit register A, and alter flags
// OpCodes: 0xED57

// LD A,R
// Copy value in 8-bit register R to 8-bit register A, and alter flags
// OpCodes: 0xED5F

void LDAIR( uint8_t* aReg, uint8_t* specReg, uint8_t* fReg, int *iff2 );

#endif // INC_8BITLOADGROUP_H 
