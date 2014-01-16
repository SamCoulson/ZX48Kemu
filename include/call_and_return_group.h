#ifndef INC_CALLANDRETUNGROUP_H
#define INC_CALLANDRETUNGROUP_H

#include <stdint.h>
//////////////////////////////////////////////////////////////////////////

// *** Call and Return group ***

// CALL nn
// Save PC to external memory stack and load in nn to PC
// OpCodes: 0xCD
void CALL( uint16_t* addrs, uint16_t* spAddrs, uint16_t* sp, uint16_t* pc );

// CALL cc,nn
// Save PC to external memory stack and load in nn to PC under a condition
// OpCodes: 0xDC, 0xFC, 0xD4, 0xC4, 0xF4, 0xEC, 0xE4, 0xCC

// 0xDC
void CALLC( uint16_t* addrs, uint16_t* spAddrs, uint16_t* sp, uint16_t* pc, uint8_t* fReg );

// 0xC4
void CALLNZ( uint16_t* addrs, uint16_t* spAddrs, uint16_t* sp, uint16_t* pc, uint8_t* fReg );

// CALL NC,nn
// 0xD4
void CALLNC( uint16_t* addrs, uint16_t* spAddrs, uint16_t* sp, uint16_t* pc, uint8_t* fReg );

// CALL Z,nn
// CAll the address if Z flag is set
// 0xCC
void CALLZ( uint16_t* addrs, uint16_t* spAddrs, uint16_t* sp, uint16_t* pc, uint8_t* fReg );

// CALL PO
// 0xF4
void CALLPO( uint16_t* addrs, uint16_t* spAddrs, uint16_t* sp, uint16_t* pc, uint8_t* fReg );

// CALL PE
// 0xEC
void CALLPE( uint16_t* addrs, uint16_t* spAddrs, uint16_t* sp, uint16_t* pc, uint8_t* fReg );

// CALL P
// 0xF4
void CALLP( uint16_t* addrs, uint16_t* spAddrs, uint16_t* sp, uint16_t* pc, uint8_t* fReg );

// CALL M
// 0xFC
void CALLM( uint16_t* addrs, uint16_t* spAddrs, uint16_t* sp, uint16_t* pc, uint8_t* fReg );

// RET
// Copy stack pointer address to HO and stack pointer+1 to LO of PC
// OpCodes: 0xC9
void RET( uint16_t* pc, uint16_t* spAddrs, uint16_t* sp );

// RET cc
// Copy stack pointer address to HO and stack pointer+1 to LO of PC	
// OpCodes: 0xF8, 0xC0, 0xF0, 0xE8, 0xE0, 0xC8

// 0xC0
void RETNZ( uint16_t* pc, uint16_t* spAddrs, uint16_t* sp, uint8_t* fReg );

// 0xE0 ***Check this***
void RETPO( uint16_t* pc, uint16_t* spAddrs, uint16_t* sp, uint8_t* fReg );

// 0xE8 ***Check this***
void RETPE( uint16_t* pc, uint16_t* spAddrs, uint16_t* sp, uint8_t* fReg );

// 0xF0 ***Check this***
void RETP( uint16_t* pc, uint16_t* spAddrs, uint16_t* sp, uint8_t* fReg );

// 0xF8 ***Check this***
void RETM( uint16_t* pc, uint16_t* spAddrs, uint16_t* sp, uint8_t* fReg );

// RET C
// Return if C is set
// OpCodes: 0xD8
void RETC( uint16_t* pc, uint16_t* spAddrs, uint16_t* sp, uint8_t* fReg );

// RET NC 
// Return on condition that the C flag is non-carry i.e. 0
// OpCodes: 0xD0
void RETNC( uint16_t* pc, uint16_t* spAddrs, uint16_t* sp, uint8_t* fReg );


// RET Z
// Return on conditio that the Z flag is 0
// OpCodes: 0xC8
void RETZ( uint16_t* pc, uint16_t* spAddrs, uint16_t *sp, uint8_t* fReg );

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
void RST( uint8_t addrs, uint16_t* spAddrs, uint16_t* sp, uint16_t* pc );

#endif // INC_CALLANDRETUNGROUP_H 
