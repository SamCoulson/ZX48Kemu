#ifndef INC_8BITARITHMETICGROUP_H
#define INC_8BITARITHMETICGROUP_H 


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// *** 8-Bit Arithmetic group*** 

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
void ADD8BitValToA( uint8_t val );

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
void SUB8BitValFromA( uint8_t val ); 	
			
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
void AND8BitValWithA( int8_t val ); 	

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

// XOR,s		
// Logical XOR an a 8-bit value with the accumulater
// XOR A,r 
// OpCodes: 0xA8, 0xA9,0xAA, 0xAB, 0xAC, 0xAD, 0xAF   
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
void XOR8BitValWithA( uint8_t val ); 	

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
void CP8BitValWithA( uint8_t val ); 	

// INC r
// Increment an 8-Bit register
// OpCodes: 0x3C, 0x04, 0x0C, 0x14, 0x1C, 0x24, 0x2C,  
void INC8BitReg( uint8_t &reg );

// INC(HL)
// Increment the contents on address pointed to by HL register
// OpCodes: 0x34
void INCAddrsOfHL( uint8_t &reg );

// INC(IX+d)
// Increment the contents of address plus offset in IX register
// OpCodes: 0xDD34
void INCAddrsOfIXOffset( uint8_t &reg );

// INC(IY+d)
// Increment the contents of address plus offset in IX register
// OpCodes: 0xFD34
void INCAddrsOfIYOffset( uint8_t &reg );

// DEC r
// Decrement an 8-Bit register
// OpCodes: 0x3C, 0x04, 0x0C, 0x14, 0x1C, 0x24, 0x2C,  
void DEC8BitReg( uint8_t &reg );

// DEC(HL)
// Decrement the contents on address pointed to by HL register
// OpCodes: 0x35
void DECAddrsOfHL();

// DEC(IX+d)
// Decrement the contents of address plus offset in IX register
// OpCodes: 0xDD34
void DECAddrsOfIXOffset( uint8_t offset );

// DEC(IY+d)
// Decrement the contents of address plus offset in IX register
// OpCodes: 0xFD34
void DECAddrsOfIYOffset( uint8_t offset );

#endif // INC_8BITARITHMETICGROUP_H 
