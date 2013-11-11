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

#endif // INC_ROTATEANDSHIFTGROUP_H  
