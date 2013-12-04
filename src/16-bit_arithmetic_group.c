#include "../include/16-bit_arithmetic_group.h"
#include "../include/util_bit_operations.h"
#include <stdio.h>

// *** 16-bit Arithmetic group***

// ADD HL,ss
// Any of the register pairs BC, DE, HL or SP are added to the contents
// of the HL register
// OpCodes: 0x09, 0x19, 0x29, 0x39
void ADD16( uint16_t* dstReg, uint16_t* srcReg, uint8_t* fReg ){
	
	// Add the regVal and hl val together and store in HL
	*dstReg += *srcReg;	
	
	// H not sure 
	
	// N is reset
	setBit( fReg, 1, 0 );

	// C is set to 1 if bit 15 carry out 
}
/*
// ADC HL,ss
// Any of the register pairs BC, DE, HL or SP with the Carry flag 
// are added to the contents of the HL register
// OpCodes: 0xED4A, 0xED5A, 0xED6A, 0x0xED7A
void ADC16BitRegToHL( uint8_t *HOreg, uint8_t *LOreg );	
*/
// SBC HL,ss
// Any of the register pairs BC, DE, HL or SP with the Carry flag 
// are subtracted to the contents of the HL register
// OpCodes: 0xED42, 0xED52, 0xED62, 0xED72
void SBC16( uint16_t *dstReg, uint16_t *srcReg, uint8_t* fReg ){

	// Subtract regVal and Carry flag from hlVal
	*dstReg -= ( *srcReg + getBit( fReg, 1) );

	// S is 1 if result is negative
	if( (int8_t)*dstReg < 0 ){
		setBit( fReg, 7, 1 );
	}else{
		setBit( fReg, 7, 0 );
	}

	// Z is set to 1 if result is 0
	if( dstReg == 0 ){
		setBit( fReg, 6, 1 );	
	}else{
		setBit( fReg, 6, 0 );
	}

	// H not sure
	
	//P/V not sure
	
	//N is set
	setBit( fReg, 2, 1 );

	// C not sure
}	
/*
// ADD IX,pp
// Any of the register pairs BC, DE, HL or SP are added to the contents
// of the IX register
// OpCodes: 0xDD09, 0xDD19, 0xDD29, 0xDD39
void ADD16BitRegToIX( uint8_t *HOreg, uint8_t *LOreg );

// ADD IY,rr
// Any of the register pairs BC, DE, HL or SP are added to the contents
// of the IY register
// OpCodes: 0xFD09, 0xFD19, 0xFD29, 0xFD39
void ADD16BitRegToIY( uint8_t &HOreg, uint8_t &LOreg );
*/
// INC ss
// Any of the register pairs BC, DE, HL or SP are incremented
// OpCodes: 0x03, 0x13, 0x23, 0x33

// INC IX
// Contents of IX register is incremnted
// OpCodes: 0xDD23

// INC IY
// Contents of IX register is incremnted
// OpCodes: 0xFD23
void INC16( uint16_t* reg ){
	++*reg;
}
// DEC ss
// Any of the register pairs BC, DE, HL or SP are decremented
// OpCodes: 0x0B, 0x1B, 0x2B, 0x3B	

// DEC IX
// Contents of IX register is decremnted
// OpCodes: 0xDD2B

// DEC IY
// Contents of IX register is decremnted
// OpCodes: 0xFD2B

void DEC16( uint16_t* reg ){
	--*reg;	
}
