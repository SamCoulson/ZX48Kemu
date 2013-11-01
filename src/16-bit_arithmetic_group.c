#include "../include/16-bit_arithmetic_group.h"

/////////////////////////////////////////////////////////////////////////////////////////////
//	
// *** 16-bit Arithmetic group***
//

// ADD HL,ss
// Any of the register pairs BC, DE, HL or SP are added to the contents
// of the HL register
// OpCodes: 0x09, 0x19, 0x29, 0x39
void ADD16BitRegToHL( uint8_t *HOreg, uint8_t *LOreg ){

	// Read the value at the address in the register
	uint8_t regVal = readByte( byteToWord( &HOreg, &LOreg ) );	

	// Read the value at the address in the HL register
	uint8_t hlVal = readByte( byteToWord( &HOreg, &LOreg ) );

	// Add the regVal and hl val together and store in HL
	uint16_t res = regVal + hlVal;

	regH = getHOByte( &res );
	regL = getLOByte( &res );
	
	// H not sure 
	
	// N is set
	setBitInByte( regF, 2, 1 );

	// C not sure 
}

// ADC HL,ss
// Any of the register pairs BC, DE, HL or SP with the Carry flag 
// are added to the contents of the HL register
// OpCodes: 0xED4A, 0xED5A, 0xED6A, 0x0xED7A
void ADC16BitRegToHL( uint8_t *HOreg, uint8_t *LOreg );	

// SBC HL,ss
// Any of the register pairs BC, DE, HL or SP with the Carry flag 
// are subtracted to the contents of the HL register
// OpCodes: 0xED42, 0xED52, 0xED62, 0xED72
void SBC16BitRegToHL( uint8_t &HOreg, uint8_t &LOreg ){

	// Read the val at the arress in the register
	uint16_t regVal = readByte( byteToWord( &HOreg, &LOreg ) ); 

	// read the value at the address in HL
	uint16_t hlVal = readByte( byteToWord( &regH, &regL ) );

	// Subtract regVal and Carry flag from hlVal
	uint16_t res = ( regVal + (int)IsBitSet( regF, 1) ) - hlVal;

	// Write value back to HL register
	regH = getHOByte( &res );
	regL = getLOByte( &res );

	// S is 1 if result is negative
	if( (int)(int16_t)res < 0 ){
		setBitInByte( regF, 8, 1 );
	}else{
		setBitInByte( regF, 8, 0 );
	}

	// Z is set to 1 if result is 0
	if( res == 0 ){
		setBitInByte( regF, 7, 1 );	
	}else{
		setBitInByte( regF, 7, 0 );
	}

	// H not sure
	
	//P/V not sure
	
	//N is set
	setBitInByte( regF, 2, 1 );	
	// C not sure
}	

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

// INC ss
// Any of the register pairs BC, DE, HL or SP are incremented
// OpCodes: 0x03, 0x13, 0x23, 0x33
void INC16BitReg( uint8_t *HOreg, uint8_t *LOreg ){
	uint16_t val = readByte( byteToWord( &HOreg, &LOreg ) );
	val++;
	
	// Write value
	HOreg = getHOByte( &val );
	LOreg = getLOByte( &val );	
}	

// INC IX
// Contents of IX register is incremnted
// OpCodes: 0xDD23
void INCIX();	

// INC IY
// Contents of IX register is incremnted
// OpCodes: 0xFD23
void INCIY();	

// DEC ss
// Any of the register pairs BC, DE, HL or SP are decremented
// OpCodes: 0x0B, 0x1B, 0x2B, 0x3B
void CPU::DEC16BitReg( uint8_t &HOreg, uint8_t &LOreg ){
	uint16_t word = byteToWord( &HOreg, &LOreg );
	word--;

	HOreg = getHOByte( &word );
	LOreg = getLOByte( &word );
}	

// DEC IX
// Contents of IX register is decremnted
// OpCodes: 0xDD2B
void CPU::DEXIX(){
	indexIX--;
}

// DEC IY
// Contents of IX register is decremnted
// OpCodes: 0xFD2B
void CPU::DECIY(){
	indexIY--;
}

