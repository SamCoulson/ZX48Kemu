#include "../include/jump_group.h"
#include "../include/util_bit_operations.h"

// *** Jump group ***

// JP nn
// Jump to the memory location using a 16-bit address specified by the combined 8-bit values  
// OpCodes: 0xC3, 0xE9
void JP( uint16_t* pc, uint16_t* addrs ){
	// Set the address the pc points to jump address minus one, to compensate for next incremnet on execution loop
	*pc = (*addrs)-1;	
}
/*
// JP cc, nn
// Jump according to the condition (cc) of the F-register to a 16-bit address 
// OpCodes: 0xC2, 0xD2, 0xE2, 0xF3 0xCA, 0xDA, 0xEA, 0xFA
void JPCondition( const char* flag, uint8_t byte1, uint8_t byte2 ){
	// first byte is LO of 16 bit address
	
	if( flag == "NZ" ){
		// If N and Z are non-zero jump to address
//		if( ( getBit( regF, 6 ) == 0 ) && ( getBit( regF, 1 ) == 0 ) ){
//			pc = byteToWord( &byte2, &byte1 );
//		}
	}
}
*/


// JR e
// Jump relative given 8-bit value
// OpCodes: 0x18
void JR( uint8_t* val, uint16_t* pc ){
	*pc += (int8_t)*val;
}

// JR C,e
// Jump relative on according to state of the carry flag
// OpCodes: 0x38
void JRC( uint16_t* pc, uint8_t* val, uint8_t* fReg ){
	// Id C flag is 0 jump relative to val
	if( getBit( fReg, 0 ) == 1 ){
		// Add value to pc
		*pc += (int8_t)*val;
	}
	// Else do nothing	
}
// JR NC,e
// Jump relative when c not 0 - Jump by val if c flag is non-zero *check this*
// OpCodes: 0x30
void JRNC( uint16_t* pc, uint8_t* val, uint8_t* fReg ){
	// Id C flag is 0 jump relative to val
	if( getBit( fReg, 0 ) == 0 ){
		// Add value to pc
		*pc += (int8_t)*val;
	}
	// Else do nothing	
}

// JR Z, e
// Jump relative when z flag is 0
// OpCodes: 0x28 
void JRZ( uint16_t* pc, uint8_t* val, uint8_t* fReg ){
	// If Z flag is 0 jump relative to val
	if( getBit( fReg, 6 ) == 0x01  ){
		// Add value to pc
		*pc += (int8_t)*val;
	}
	// Else do nothing and move to next instruction	
}


// JR NZ, e
// Jump relative when z not 0 - Jump by val if Z in flags is non-zero
// OpCodes: 0x20
void JRNZ( uint16_t* pc, uint8_t* val, uint8_t* fReg ){

	// IF Z is 0 jump
	if( getBit( fReg, 6 ) == 0x00 ){
		*pc += (int8_t)*val;	
	}
	// If not, do nothing and execute next instruction
}

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
void DJNZ( uint8_t* disp, uint8_t* bReg, uint16_t* pc ){
	
	--*bReg;
		
	if( *bReg != 0x00 ){
		*pc += (int8_t)*disp;
	}
}

