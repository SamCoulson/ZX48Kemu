#include "../include/cpu.h"
// *** Jump group ***

// JP nn
// Jump to the memory location using a 16-bit address specified by the combined 8-bit values  
// OpCodes: 0xC3
void CPU::JP( uint8_t byte1, uint8_t byte2 ){
	pc = byteToWord( &byte1, &byte2  );
}

// JP cc, nn
// Jump according to the condition (cc) of the F-register to a 16-bit address 
// OpCodes: 0xC2, 0xD2, 0xE2, 0xF3 0xCA, 0xDA, 0xEA, 0xFA
void CPU::JPCondition( const char* flag, uint8_t byte1, uint8_t byte2 ){
	// first byte is LO of 16 bit address
	
	if( flag == "NZ" ){
		// If N and Z are non-zero jump to address
		if( ( IsBitSet( regF, 7 ) != true ) && ( IsBitSet( regF, 2 ) != true ) ){
			pc = byteToWord( &byte2, &byte1 );
		}
	}
}



// JR e
// Jump relative given 8-bit value
// OpCodes: 0x18
void JR( uint8_t val );

// JR C,e
// Jump relative on according to state of the carry flag
// OpCodes: 0x38
void JRC( uint8_t val );

// JR NC,e
// Jump relative when c not 0 - Jump by val if c flag is non-zero *check this*
// OpCodes: 0x30
void JRNC( uint8_t val );

// JR Z, e
// Jump relative when z flag is 0
// OpCodes: 0x28 
void JRZ( uint8_t val );

// JR NZ, e
// Jump relative when z not 0 - Jump by val if Z in flags is non-zero
// OpCodes: 0x20
void CPU::JRNZ( int8_t val ){
	// IF Z is 0 jump
	
	if( IsBitSet( regF, 7 ) != true ){
		// jump to val but -2 from the value first,
		// 1 for because jump happens from the
		// opcode location and 2 because the loop will autoincrement on
		// execution of next instruction
		val +=2;		
		pc += val;	
	}

	std::cout << "HL reg " << std::hex << (int)regH << (int)regL << std::endl;
	// If not, do nothing
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
void DJNZ( uint8_t val );

