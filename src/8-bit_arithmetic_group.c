#include "../include/8-bit_arithmetic_group.h" 
#include "../include/util_bit_operations.h"
// *** 8-Bit Arithmetic group*** 
/*
// ADD A,s
// Adds an a 8-bit value to the accumulater
// ADD A,r 
// OpCodes: 0x80, 0x81, 0x82, 0x83 ,0x84 ,0x85 ,0x87
// ADD A,n
// Add an 8-bit integer to register A
// OpCodes: 0xC6
// ADD A,(HL)
// Add the value at the address of HL to register A
// OpCodes: 0x86
// ADD A,(IX+d)
// Add the value at address of IX plus offset to register A
// OpCodes: 0xDD86 
// ADD A,(IY+d)
// Add the value at address of IX plus offset to register A
// OpCodes: 0xFD86 	
void ADD( uint8_t *dstReg, uint8_t val ){
	// Add the value to the register A
	dstReg += val;

	// Alter flags
	
	// Is result negative set S flag
	if( dstReg < 0 ){
		setBitInByte( regA, 8, 1 );
	}else{
		setBitInByte( regA, 8, 0 );
	}

	// If zero set z flag
	if( regA == 0 ){
		setBitInByte( regA, 7, 1 );
	}else{
		setBitInByte( regA, 7, 0 );	
	}


	// Supposed to set the H bit?? Not sure here
	//
	
        // P/V is set if overflow
        
	// N is reset
	setBitInByte( regA, 2, 0 );

	// C is set if carry from bit 7
	

	pc++;
}

// ADC A,r
// Add an 8-bit integer + C Flag to register A  
// OpCodes: 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8F
// ADC A,n
// Add an 8-bit integer + C Flag to register A
// OpCodes: 0xCE
// ADC A,(HL)
// Add the value at the address of HL + C Flag to register A
// OpCodes: 0x8E
// ADC A,(IX+d)
// Add the value at address of IX plus offset + C Flag to register A
// OpCodes: 0xDD8E
// ADC A,(IY+d)
// Add the value at address of IX plus offset + C Flag to register A
// OpCodes: 0xFD8E
void ADC8BitValToA( uint8_t val );
*/
// SUB A,s
// Subtract an a 8-bit value from the accumulater
// SUB A,r 
// OpCodes: 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x97 
// SUB A,n
// Subtract an 8-bit integer from register A
// OpCodes: 0xD6
// SUB A,(HL)
// Subtract the value at the address of HL from register A
// OpCodes: 0x96
// SUB A,(IX+d)
// Subtract the value at address of IX plus offset from register A
// OpCodes: 0xDD96
// SUB A,(IY+d)
// Subtract the value at address of IX plus offset from register A
// OpCodes: 0xFD96
void SUB( uint8_t* aReg, uint8_t* val, uint8_t* fReg ){
	*aReg -= *val;

	// S is 1 if result is negative
	if( *aReg < 0x00 ){
		setBit( fReg, 7, 1 );
	}else{
		setBit( fReg, 7, 0 );
	}
	// Z is 1 if result is 0
        if( *aReg == 0x00 ){
		setBit( fReg, 6, 1 );
	}else{
		setBit( fReg, 6, 0 );
	}
	// H is 1 if borrow bit 4
	// P/V is 1 if overflow
	// N is set to 1
	setBit( fReg, 1, 1 );	
	// C is 1 if borrow	
}	
/*			
// SBC A,r
// Subtract an 8-bit integer - C Flag from register A  
// OpCodes: 0x9F, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D 
// SBC A,n
// Subtract an 8-bit integer - C Flag from register A
// OpCodes: 0xDE
// SBC A,(HL)
// Subtract the value at the address of HL - C Flag from register A
// OpCodes: 0x9E
// SBC A,(IX-d)
// Subtract the value at address of IX plus offset - C Flag from register A
// OpCodes: 0xDD9E
// ADD A,(IY-d)
// Subtract the value at address of IX plus offset - C Flag from register A
// OpCodes: 0xFD9E
void SBC8BitValFromA( uint8_t val );
*/
// AND A,s
// Logical AND an a 8-bit value with the accumulater
// AND A,r 
// OpCodes: 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA7  
// AND A,n
// Logical AND an 8-bit integer with register A
// OpCodes: 0xE6
// AND A,(HL)
// Logical AND the value at the address of HL with register A
// OpCodes: 0xA6
// AND A,(IX+d)
// Logical AND the value at address of IX plus offset with register A
// OpCodes: 0xDDA6 
// AND A,(IY+d)
// Logical AND the value at address of IX plus offset with register A
// OpCodes: 0xFDA6
void AND( uint8_t* reg, uint8_t* val, uint8_t* fReg ){
	*reg &= *val;

	//printf("regA is %d", (int)(int8_t)regA);

	// If result negative set S to 1 else 0
	if( (int8_t)*reg < 0 ){	
		setBit( fReg, 7, 1 );
	}else{
		setBit( fReg, 7, 0 );
	}

	// Z is set to 1 if result is zero else 0
	if( *reg == 0 ){
		setBit( fReg, 6, 1 );	
	}else{
		setBit( fReg, 6, 0 );
	}

	// H is set
	setBit( fReg, 4, 1 );

	// P/V not sure
	
	// N and C are reset
	setBit( fReg, 1, 0 );
	setBit( fReg, 0, 0 );
}	
/*
// OR,s		
// OR A,s
// Logical OR an a 8-bit value with the accumulater
// OR A,r 
// OpCodes: 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB7  
// OR A,n
// Logical OR an 8-bit integer with register A
// OpCodes: 0xF6
// OR A,(HL)
// Logical OR the value at the address of HL with register A
// OpCodes: 0xB6
// OR A,(IX+d)
// Logical OR the value at address of IX plus offset with register A
// OpCodes: 0xDDB6
// OR A,(IY+d)
// Logical OR the value at address of IX plus offset with register A
// OpCodes: 0xFDB6
void OR8BitValWithA( uint8_t val ); 	
*/
// XOR,s		
// Logical XOR an a 8-bit value with the accumulater
// XOR A,r 
// OpCodes: 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAF   
// XOR A,n
// Logical XOR an 8-bit integer with register A
// OpCodes: 0xEE
// XOR A,(HL)
// Logical XOR the value at the address of HL with register A
// OpCodes: 0xAE
// XOR A,(IX+d)
// Logical XOR the value at address of IX plus offset with register A
// OpCodes: 0xDDAE
// XOR A,(IY+d)
// Logical XOR the value at address of IX plus offset with register A
// OpCodes: 0xFDAE
void XOR( uint8_t *dstReg, uint8_t* srcVal, uint8_t* fReg ){
	// Exclusive-OR the val with register A
	*dstReg ^= *srcVal;

	// S is one if result is negative
	if( (int8_t)*dstReg < 0 ){
		setBit( fReg, 7, 1 );
	}else{
		setBit( fReg, 7, 0 );
	}

	// Z is one if result is zero
	if( *dstReg == 0 ){
		setBit( fReg, 6, 1 );	
	}else{
		setBit( fReg, 6, 1 );
	}

	// H is set to zero
	setBit( fReg, 4, 0 );	

	// P/V is set if parity is even
	
	// N is set to zero
	setBit( fReg, 1, 0 );	
	
	// C is set to zero
	setBit( fReg, 0, 0 );	 
} 	

// CP,s		
// Logical CP an a 8-bit value with the accumulater
// CP A,r 
// OpCodes: 0xB8, 0xB9,0xBA, 0xBB, 0xBC, 0xBD, 0xBF    
// CP A,n
// Logical CP an 8-bit integer with register A
// OpCodes: 0xFE
// CP A,(HL)
// Logical CP the value at the address of HL with register A
// OpCodes: 0xBE
// CP A,(IX+d)
// Logical CP the value at address of IX plus offset with register A
// OpCodes: 0xDDBE 
// CP A,(IY+d)
// Logical CP the value at address of IX plus offset with register A
// OpCodes: 0xFDBE
void CP( uint8_t* reg, uint8_t* val, uint8_t* fReg ){
	// If the values are equal set the Z flag to 1 and S flag to 0
	if( *val == *reg ){
		setBit( fReg, 6, 1 ); // Z
		setBit( fReg, 7, 0 ); // S
	}else{
		setBit( fReg, 6, 0 );
		setBit( fReg, 7, 1 );
	}

	// ** Overflow bits need to be set ?? Not sure how yet **

	// N is set
	setBit( fReg, 1, 1 );	
}	

// INC r
// Increment an 8-Bit register
// OpCodes: 0x3C, 0x04, 0x0C, 0x14, 0x1C, 0x24, 0x2C,  
// INC(HL)
// Increment the contents on address pointed to by HL register
// OpCodes: 0x34
// INC(IX+d)
// Increment the contents of address plus offset in IX register
// OpCodes: 0xDD34
// INC(IY+d)
// Increment the contents of address plus offset in IX register
// OpCodes: 0xFD34
void INC( uint8_t* val ){
	++*val;
}

// DEC r
// Decrement an 8-Bit register
// OpCodes: 0x05, 0x15, 0x25, 0x0D, 0x1D, 0x2D, 0x3D  
// DEC(HL)
// Decrement the contents on address pointed to by HL register
// OpCodes: 0x35
// DEC(IX+d)
// Decrement the contents of address plus offset in IX register
// OpCodes: 0xDD35
// DEC(IY+d)
// Decrement the contents of address plus offset in IX register
// OpCodes: 0xFD35
void DEC( uint8_t* val ){
	--*val;
}
