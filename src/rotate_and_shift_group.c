#include "../include/rotate_and_shift_group.h"
#include "../include/util_bit_operations.h"

// RLCA
// Contents of register A are rotated left one position, the content of bit 7 is copied to the
// carry flag and to bit0.
// OpCodes: 0x07
void RLCA( uint8_t* aReg, uint8_t* fReg ){
	// Copy contents of bit 7 to carry flag
	setBit( fReg, 0, getBit( aReg, 7 ) ); 
	// Shift the bits in register left 1
	*aReg <<= 1;
	// Copy contents of carry flag to bit 0 of register value
	setBit( aReg, 0, getBit( fReg, 7 ) );
}

// RLA
// Contents of register A are rotated left 1-bit position through the carry flag.  The previous
// content of the carry flag is copied to bit 0.
// OpCodes: 0x17
void RLA( uint8_t* aReg, uint8_t* fReg ){
	// Save contents of current C flag
	uint8_t cFlag = getBit( fReg, 0 );
	// Copy bit 0 of register contents into carry flag
	setBit( fReg, 0, getBit( aReg, 0 ) );
	// Bit shift right 1 place
	*aReg <<= 1;
	// Copy previous carry flag into 7 of register
	setBit( aReg, 7, cFlag );
}

// RRCA
// Contents of register A rotated 1-bit position to the right through the Carry flag 
// OpCodes: 0x0F
void RRCA( uint8_t* aReg, uint8_t* fReg){
	// Copy bit 0 of register contents into carry flag
	setBit( fReg, 0, getBit( aReg, 0 ) );
	// Bit shift right 1 place
	*aReg >>= 1;
	// Copy carry flag into 7 of A register
	setBit( aReg, 7, getBit( fReg, 0 ) );

	// Reset H
	setBit( fReg, 4, 0 );
	// Reset N
	setBit( fReg, 1, 0 );	
}

// RRA
// Contents of register A rotated 1-bit position the previous contents 
// of the carry flag are copied to bit 7 of register
// OpCodes: 0x1F
void RRA( uint8_t* aReg, uint8_t* fReg){
	// Save contents of current C flag
	uint8_t cFlag = getBit( fReg, 0 );
	// Copy bit 0 of register contents into carry flag
	setBit( fReg, 0, getBit( aReg, 0 ) );
	// Bit shift right 1 place
	*aReg >>= 1;
	// Copy previous carry flag into 7 of register
	setBit( aReg, 7, cFlag );
}

// RLC
// Contents of register are rotated left one position, the content of bit 7 is copied to the
// carry flag and to bit 0.
// OpCodes: 0xCB01 - 07
void RLC( uint8_t* reg, uint8_t* fReg ){
	// Copy contents of bit 7 to carry flag
	setBit( fReg, 0, getBit( reg, 7 ) ); 
	// Shift the bits in register left 1
	*reg <<= 1;
	// Copy contents of carry flag to bit 0 of register value
	setBit( reg, 0, getBit( fReg, 7 ) );
}
