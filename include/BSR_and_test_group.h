#ifndef INC_BSRANDTESTGROUP_H
#define INC_BSRANDTESTGROUP_H

#include <stdint.h>

/////////////////////////////////////////////////////////////////////////////////////
//
// Bit set, Reset, and test group
//

// BIT b,r
// Test bit b in register r
// OpCodes: 0xCB

// BIT b,(HL)
// Test bit b in the memory location specified by HL
// OpCodes: 0xCB

// BIT b(IX+d)
// Test bit b in memory location specifed by IX plus 2's complement offset d
// OpCodes: 0xDDCB

// BIT b(IY+d)
// Test bit b in memory location specifed by IX plus 2's complement offset d
// OpCodes: 0xFDCB
void BIT( uint8_t bit, uint8_t* byte, uint8_t* fReg );

// SET b,r
// Set bit b in register r
// OpCodes: 0xCB

// SET b,(HL)
// Set bit b in memory location specified by HL
// OpCodes: 0xCB
 
// SET b,(IX+d)
// Set bit b in memory location specifed by IX plus 2's complement offset d
// OpCodes: 0xDD
 
// SET b,(IY+d)
// Set bit b in memory location specifed by IY plus 2's complement offset d
// OpCodes: 0xFD
void SET( uint8_t bit, uint8_t* byte );
 
// RES b,m
// Bit b in regiser is reset
// opCodes: 0xCB 0xDD 0xFD

// RES b,(HL)
// Set bit b in memory location specified by HL
// OpCodes: 0xCB
 
// RES b,(IX+d)
// Set bit b in memory location specifed by IX plus 2's complement offset d
// OpCodes: 0xDD
 
// RES b,(IY+d)
// Set bit b in memory location specifed by IY plus 2's complement offset d
// OpCodes: 0xFD
void RES( uint8_t bit, uint8_t* byte );
 

#endif // INC_BSRANDTESTGROUP_H 
