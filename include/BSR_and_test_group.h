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
void BitIn8BitReg( uint8_t bit, uint8_t *reg );

// BIT b,(HL)
// Test bit b in the memory location specified by HL
// OpCodes: 0xCB
void BitInAddrsOfHL( uint8_t bit );

// BIT b(IX+d)
// Test bit b in memory location specifed by IX plus 2's complement offset d
// OpCodes: 0xDDCB
void BitInAddrsOfIXOffset( uint8_t bit, uint8_t offset );

// BIT b(IY+d)
// Test bit b in memory location specifed by IX plus 2's complement offset d
// OpCodes: 0xFDCB
void BitInAddrsOfIYOffset( uint8_t bit, int8_t offset );

// SET b,r
// Set bit b in register r
// OpCodes: 0xCB
void SetBitIn8BitReg( uint8_t bit, uint8_t *reg );

// SET b,(HL)
// Set bit b in memory location specified by HL
// OpCodes: 0xCB
void SetBitInAddresOfHL( uint8_t bit );
 
// SET b,(IX+d)
// Set bit b in memory location specifed by IX plus 2's complement offset d
// OpCodes: 0xDD
void SetBitInAddrsOfIXOffset( uint8_t bit, uint8_t offset );
 
// SET b,(IY+d)
// Set bit b in memory location specifed by IY plus 2's complement offset d
// OpCodes: 0xFD
void SetBitInAddrsOfIYOffset( uint8_t bit, uint8_t offset );
 
// RES b,m
// Bit b in regiser is reset
// opCodes: 0xCB 0xDD 0xFD
void ResetBitIn16BitReg( uint8_t bit, uint8_t *regHO );

// RES b,(HL)
// Set bit b in memory location specified by HL
// OpCodes: 0xCB
void ResetBitInAddresOfHL( uint8_t bit );
 
// RES b,(IX+d)
// Set bit b in memory location specifed by IX plus 2's complement offset d
// OpCodes: 0xDD
void ResetBitInAddrsOfIXOffset( uint8_t bit, uint8_t offset );
 
// RES b,(IY+d)
// Set bit b in memory location specifed by IY plus 2's complement offset d
// OpCodes: 0xFD
void ResetBitInAddrsOfIYOffset( uint8_t bit, uint8_t offset );
 

#endif // INC_BSRANDTESTGROUP_H 
