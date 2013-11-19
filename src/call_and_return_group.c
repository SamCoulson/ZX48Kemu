#include "../include/call_and_return_group.h"
#include "../include/util_bit_operations.h"
//////////////////////////////////////////////////////////////////////////

// *** Call and Return group ***

// CALL nn
// Save PC to external memory stack and load in nn to PC
// OpCodes: 0xCD
void CALL( uint16_t* addrs, uint16_t* spAddrs, uint16_t* sp, uint16_t* pc ){
		
	// Make room for the pc address
	--spAddrs;
	*sp-=2;

	// Save the contents of the PC on the stack pointer
	*spAddrs = *pc;

	// Set the pc to point to the address -1 to compensate the autoincrment on next loop
	*pc = *addrs-1;	
}

// CALL cc,nn
// Save PC to external memory stack and load in nn to PC under a condition
// OpCodes: 0xDC, 0xFC, 0xD4, 0xC4, 0xF4, 0xEC, 0xE4

// CALL Z,nn
// CAll the address if Z flag is set
// OpCodes: 0xCC
void CALLZ( uint16_t* addrs, uint16_t* spAddrs, uint16_t* sp, uint16_t* pc, uint8_t* fReg ){
	
	if( getBit( fReg, 6 ) == 0x01 ){
		// Make room for the pc address
		--spAddrs;
		*sp-=2;

		// Save the contents of the PC on the stack pointer
		*spAddrs = *pc;

		// Set the pc to point to the address -1 to compensate the autoincrment on next loop
		*pc = *addrs-1;
	}	
}	

// RET
// Copy stack pointer address to PC
// OpCodes: 0xC9
void RET( uint16_t* pc, uint16_t* spAddrs, uint16_t* sp ){
		
	// Copy addres on stack to PC
	*pc = *spAddrs;
	*sp+=2;
}

// RET cc
// Copy stack pointer address to HO and stack pointer+1 to LO of PC	
// OpCodes: 0xF8, 0xC0, 0xF0, 0xE8, 0xE0, 0xC8

// RET C
// Return if C is set
// OpCodes: 0xD8
void RETC( uint16_t* pc, uint16_t* spAddrs, uint16_t* sp, uint8_t* fReg ){
	if( getBit( fReg, 0 ) == 0x01 ){	
		*pc = *spAddrs;
		*sp+=2;	
	}
}


// RET NC 
// Return on condition that the C flag is non-carry i.e. 0
// OpCodes: 0xD0
void RETNC( uint16_t* pc, uint16_t* spAddrs, uint16_t* sp, uint8_t* fReg ){
	if( getBit( fReg, 0 ) == 0x00 ){	
		*pc = *spAddrs;
		*sp+=2;	
	}	
}

// RET Z
// Return on conditio that the Z flag is 0
// OpCodes: 0xC8
void RETZ( uint16_t* pc, uint16_t* spAddrs, uint16_t *sp, uint8_t* fReg ){
	if( getBit( fReg, 6 ) == 0x01 ){
		*pc = *spAddrs;
		*sp+=2;
	}	
}

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
void RST( uint8_t addrs, uint16_t* spAddrs, uint16_t* sp, uint16_t* pc ){
	// Make room for the pc address
	--spAddrs;
	*sp-=2;

	// Save the contents of the PC on the stack pointer
	*spAddrs = *pc;

	// Set the pc to point to the address -1 to compensate the autoincrment on next loop
	*pc = addrs-1;	
}


