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

// 0xDC
void CALLC( uint16_t* addrs, uint16_t* spAddrs, uint16_t* sp, uint16_t* pc, uint8_t* fReg ){
	if( getBit( fReg, 0 ) == 0x01 ){
		// Make room for the pc address
		--spAddrs;
		*sp-=2;

		// Save the contents of the PC on the stack pointer
		*spAddrs = *pc;

		// Set the pc to point to the address -1 to compensate the autoincrment on next loop
		*pc = *addrs-1;
	}
}

// CALL NZ - Really not sure why these work when expression evaluates to 0 instead of 1
// Call if Z flag is reset
// 0xC4
void CALLNZ( uint16_t* addrs, uint16_t* spAddrs, uint16_t* sp, uint16_t* pc, uint8_t* fReg ){
	if( getBit( fReg, 1 ) == 0x00 ){
		// Make room for the pc address
		--spAddrs;
		*sp-=2;

		// Save the contents of the PC on the stack pointer
		*spAddrs = *pc;

		// Set the pc to point to the address -1 to compensate the autoincrment on next loop
		*pc = *addrs-1;
	}
}

// CALL NC,nn
// 0xD4
void CALLNC( uint16_t* addrs, uint16_t* spAddrs, uint16_t* sp, uint16_t* pc, uint8_t* fReg ){
	if( getBit( fReg, 0 ) != 0x00 ){
		// Make room for the pc address
		--spAddrs;
		*sp-=2;

		// Save the contents of the PC on the stack pointer
		*spAddrs = *pc;

		// Set the pc to point to the address -1 to compensate the autoincrment on next loop
		*pc = *addrs-1;
	}	
}

// CALL PO
// 0xE4
void CALLPO( uint16_t* addrs, uint16_t* spAddrs, uint16_t* sp, uint16_t* pc, uint8_t* fReg ){
	/*
	if( getBit( fReg, 0 ) != 0x00 ){
		// Make room for the pc address
		--spAddrs;
		*sp-=2;

		// Save the contents of the PC on the stack pointer
		*spAddrs = *pc;

		// Set the pc to point to the address -1 to compensate the autoincrment on next loop
		*pc = *addrs-1;
	}*/
	printf("Not implemented\n");	
}

// 0xEC
void CALLPE( uint16_t* addrs, uint16_t* spAddrs, uint16_t* sp, uint16_t* pc, uint8_t* fReg ){
// Parity even
	printf("Not implemented\n");		
}

// 0xF4
void CALLP( uint16_t* addrs, uint16_t* spAddrs, uint16_t* sp, uint16_t* pc, uint8_t* fReg ){
// Sign positive
	printf("Not implemented\n");	
}

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

// 0xFC
void CALLM( uint16_t* addrs, uint16_t* spAddrs, uint16_t* sp, uint16_t* pc, uint8_t* fReg ){
	// Sign Negative
	printf("Not implemented\n");	
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

// 0xE0
// When parity is ODD ***Check this*** P/V is reset
void RETPO( uint16_t* pc, uint16_t* spAddrs, uint16_t* sp, uint8_t* fReg ){
	///if( getBit( fReg, 2 ) != 0x00 ){	
	//	*pc = *spAddrs;
	//	*sp+=2;	
	//}
	printf("Not implemented\n");		
}

// 0xE8 ***Check this*** P/V is set
void RETPE( uint16_t* pc, uint16_t* spAddrs, uint16_t* sp, uint8_t* fReg ){
	//if( getBit( fReg, 2 ) != 0x00 ){	
	//	*pc = *spAddrs;
	//	*sp+=2;	
	//}
	printf("Not implemented\n");		
}

// 0xF0 ***Check this***
void RETP( uint16_t* pc, uint16_t* spAddrs, uint16_t* sp, uint8_t* fReg ){
	printf("Not implemented\n");	

}

// 0xF8 ***Check this***
void RETM( uint16_t* pc, uint16_t* spAddrs, uint16_t* sp, uint8_t* fReg ){
	// Sign negative
	printf("Not implemented\n");	
}

// RET NZ
// Return if Z flag is zero /* Completely got me, surely non-zero means anything but 0 */
// OpCodes: 0xC0
void RETNZ( uint16_t* pc, uint16_t* spAddrs, uint16_t* sp, uint8_t* fReg ){
	if( getBit( fReg, 6 ) == 0x00 ){	
		*pc = *spAddrs;
		*sp+=2;	
	}	
}

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
// Return on condition that the Z flag is 1
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


