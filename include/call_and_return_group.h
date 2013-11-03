#ifndef INC_CALLANDRETUNGROUP_H
#define INC_CALLANDRETUNGROUP_H

#include <stdint.h>
//////////////////////////////////////////////////////////////////////////

// *** Call and Return group ***

// CALL nn
// Save PC to external memory stack and load in nn to PC
// OpCodes: 0xCD
void CALL( uint8_t HObyte, uint8_t LObyte );

// CALL cc,nn
// Save PC to external memory stack and load in nn to PC under a condition
// OpCodes: 0xDC, 0xFC, 0xD4, 0xC4, 0xF4, 0xEC, 0xE4, 0xCC
void CALLCond( uint8_t HObyte, uint8_t LObyte );		

// RET
// Copy stack pointer address to HO and stack pointer+1 to LO of PC
// OpCodes: 0xC9
void RET();

// RET cc
// Copy stack pointer address to HO and stack pointer+1 to LO of PC	
// OpCodes: 0xD8, 0xF8, 0xD0, 0xC0, 0xF0, 0xE8, 0xE0, 0xC8
void RETCond();

// RETI
// Return from interrupt
// OpCodes: 0xED4D
void RETI();

// RETN
// Return from non-maskable interrupt
// OpCodes: 0xED45
void RETN();

// RST p
// Execute page zero routines
// OpCodes: 0xC7, 0xCF, 0xD7, 0xDF, 0xE7, 0xEF, 0xF7, 0xFF 
void RST( uint8_t routine );

#endif // INC_CALLANDRETUNGROUP_H 