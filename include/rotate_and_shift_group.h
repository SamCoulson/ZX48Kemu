#ifndef INC_ROTATEANDSHIFTGROUP_H
#define INC_ROTATEANDSHIFTGROUP_H

#include <stdint.h>
// RRCA
// Contents of register A rotated 1-bit position to the right through the Carry flag 
// OpCodes: 0x0F
void RRCA( uint8_t* aReg, uint8_t* fReg);

// RRA
// Contents of register A rotated 1-bit position through the carry flag
// OpCodes: 0x1F
void RRA( uint8_t* aReg, uint8_t* fReg);

// RLC
// Contents of register are rotated left one position, the content of bit 7 is copied to the
// carry flag and to bit 0.
// OpCodes: 0xCB01
void RLC( uint8_t* reg, uint8_t* fReg );

#endif // INC_ROTATEANDSHIFTGROUP_H  
