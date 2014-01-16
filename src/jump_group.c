#include "../include/jump_group.h"
#include "../include/util_bit_operations.h"

// *** Jump group ***

// JP nn
// Jump to the memory location using a 16-bit address specified by the combined 8-bit values  
// OpCodes: 0xC3, 0xE9
void JP( uint16_t* pc, uint16_t* addrs ){
	// Set the address the pc points to jump address in the minus one, to compensate for next incremnet on execution loop
	*pc = (*addrs)-1;	
}

// JP cc, nn
// Jump according to the condition (cc) of the F-register to a 16-bit address 
// OpCodes: 0xD2, 0xE2, 0xF3 0xCA, 0xDA, 0xEA, 0xFA

// 0xCA
void JPZ( uint16_t* pc, uint16_t* addrs, uint8_t* fReg ){
	// If Z flag set i.e. Z == 1
	if( getBit( fReg, 6 ) == 0x01 ){
		// Add value to pc
		*pc = (*addrs)-1;
	}
	// Else do nothing	
}
// 0xD2
void JPNC( uint16_t* pc, uint16_t* addrs, uint8_t* fReg ){
	// If C flag is non-carry i.e. C == 0
	if( getBit( fReg, 0 ) == 0x00 ){
		// Add value to pc
		*pc = (*addrs)-1;
	}
	// Else do nothing	
}

// 0xDA
void JPC( uint16_t* pc, uint16_t* addrs, uint8_t* fReg ){
	// If C flag is carry i.e. C == 1
	if( getBit( fReg, 0 ) == 0x01 ){
		// Add value to pc
		*pc = (*addrs)-1;
	}
	// Else do nothing	
}

// 0xE2 ***Check this***
void JPPO( uint16_t* pc, uint16_t* addrs, uint8_t* fReg ){
	// If parity is odd
	//if( getBit( fReg, 0 ) == 0x01 ){
	//	// Add value to pc
	//	*pc = (*addrs)-1;
	//}
	// Else do nothing	
}

// 0xEA ***Check this***
void JPPE( uint16_t* pc, uint16_t* addrs, uint8_t* fReg ){
	// If parity bit is even

}

// 0xF2
void JPP( uint16_t* pc, uint16_t* addrs, uint8_t* fReg ){
	

}

// 0xFA
void JPM( uint16_t* pc, uint16_t* addrs, uint8_t* fReg ){
	// Sign Negative
}

// JPNZ,nn
// Jump to address when Z flag is non-zero
// OpCodes: 0xC2 
void JPNZ( uint16_t* pc, uint16_t* addrs, uint8_t* fReg ){
	// If Z flag is non-zero i.e z != 0
	if( getBit( fReg, 6 ) != 0x00 ){
		// Add value to pc
		*pc = (*addrs)-1;
	}
	// Else do nothing		
}

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
	// If C flag is 0 jump relative to val
	if( getBit( fReg, 0 ) == 0x01 ){
		// Add value to pc
		*pc += (int8_t)*val;
	}
	// Else do nothing	
}
// JR NC,e
// Jump relative when C is 0 
// OpCodes: 0x30
void JRNC( uint16_t* pc, uint8_t* val, uint8_t* fReg ){
	// If C flag is 0 jump relative to val
	if( getBit( fReg, 0 ) == 0x00 ){
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

