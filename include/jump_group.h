#ifndef INC_JUMPGROUP_H
#define INC_JUMPGROUP_H

#include <stdint.h>

/////////////////////////////////////////////////////////////////////////////////////////////
//
// *** Jump group ***

// JP nn
// Jump to the memory location using a 16-bit address specified by the combined 8-bit values  
// OpCodes: 0xC3
void JP( uint8_t byte1, uint8_t byte2 );

// JP cc, nn
// Jump according to the condition (cc) of the F-register to a 16-bit address 
// OpCodes: 0xC2, 0xD2, 0xE2, 0xF3 0xCA, 0xDA, 0xEA, 0xFA
void JPCondition( const char* flag, uint8_t byte1, uint8_t byte2 );

// JR e
// Jump relative given 8-bit value
// OpCodes: 0x18
void JR( uint8_t val );

// JR C,e
// Jump relative on according to state of the carry flag
// OpCodes: 0x38
void JRC( uint8_t val );

// JR NC,e
// Jump relative when c not 0 - Jump by val if c flag is non-zero *check this*
// OpCodes: 0x30
void JRNC( int8_t val );

// JR Z, e
// Jump relative when z flag is 0
// OpCodes: 0x28 
void JRZ( uint8_t val );

// JR NZ, e
// Jump relative when z not 0 - Jump by val if Z in flags is non-zero
// Note: has signed int type because value may be + or -
// OpCodes: 0x20
void JRNZ( int8_t val );

// JP(HL)
// Load program counter with contents of address pointed to by HL register
// OpCodes: 0xE9
void JPHL();

// JP(IX)
// Load program counter with contents of address pointed to by IX register
// OpCodes: 0xDDE9
void JPIX(); 

// JP(IY)
// Load program counter with contents of address pointed to by IY register
// OpCodes: 0xFDE9
void JPIY();		

// DJNZ, e
// Jump on the condition of B reg being non-zero
// OpCodes: 0x10
void DJNZ( uint8_t val );

#endif // INC_JUMPGROUP_H  
