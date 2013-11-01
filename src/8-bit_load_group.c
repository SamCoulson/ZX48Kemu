#include "../include/8-bit_load_group.h"
//#include "../include/util_bit_operations.h"
#include <stdio.h>

// LOAD data methods for 8-bit groups

// *** 8-BIT LOAD GROUP ***
//
// LD r,r'
// Copy 8-bit register value to 8-bit register
// OpCodes: 0x40, 0x50, 0x60, 0x41, 0x51, 0x61, 0x42, 0x52, 0x62, 0x43, 0x53, 0x63
//			0x44, 0x54, 0x64, 0x45, 0x55, 0x65, 0x47, 0x57, 0x67, 0x48, 0x58, 0x68, 0x78
//			0x49, 0x59, 0x69, 0x79, 0x4A, 0x5A, 0x6A, 0x7A
//			0x4B, 0x5B, 0x6B, 0x7B, 0x4C, 0x5C, 0x6C, 0x7C
//			0x4D, 0x5D, 0x6D, 0x7D, 0x4F, 0x5F, 0x6F, 0x7F
void LD8BitRegTo8BitReg( uint8_t *dstReg, uint8_t *srcReg ){
	// Copy the source register values into the destination register
	dstReg = srcReg;
}

// LD r,n
// Copy 8-bit constant value to 8-bit register
// OpCodes: 0x0E, 0x1E, 0x2E, 0x3E, 0x06, 0x16, 0x26
void LD8BitIntTo8BitReg( uint8_t &dst, uint8_t src ){
	// Copy the source values into the registers
	dst = src;
}

// LD r,(HL)
// Copy value from address in HL to 8-bit register
// OpCodes: 0x4E, 0x5E, 0x6E, 0x7E, 0x46, 0x56, 0x66
void LDAddrsOfHLTo8BitReg( uint8_t &reg ){
	// Read the value of the PC to determine which operation we are doing
	reg = readByte(byteToWord(&regH, &regL));
	
}

// LD r,(IX+d)
// Copy value from address in IX+d to 8-bit register
// OpCodes: 0xDD46, 0xDD4E, 0xDD56, 0xDD5E, 0xDD66, 0xDD6E, 0xDD7E
void LDAddrsOfIXdTo8BitReg( uint8_t &reg, uint8_t disp ){
	reg = readByte( indexIX+disp );
	
}

// LD r,(IY+d)
// Copy value from address in IY+d HL to 8-bit register
// OpCodes: 0xFD46, 0xFD4E, 0xFD56, 0xFD5E, 0xFD66, 0xFD6E, 0xFD7E
void LDAddrsOfIYdTo8BitReg( uint8_t &reg, uint8_t disp ){
	reg = readByte( indexIY+disp );
}

// LD (HL), r
// Copy value of 8-bit register to address in HL
// OpCodes: 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x77
void LD8BitRegToAddrsOfHL( uint8_t &reg ){
	// Read the value of the PC to determine which operation we are doing
	writeByte(byteToWord(&regH, &regL), reg);
}

// LD (IX+d),r
// Copy value from 8-bit register to address in IX+d
// OpCodes: 0xFD70, 0xFD71, 0xFD72, 0xFD73, 0xFD74, 0xFD75, 0xFD77
void LD8BitRegToAddrsOfIXd( uint8_t &reg, uint8_t disp ){
	writeByte( indexIX + disp, reg );
}

// LD (IY+d),r
// Copy value from 8-bit register to address in IY+d 
// OpCodes: 0xDD70, 0xDD71, 0xDD72, 0xDD73, 0xDD74, 0xDD75, 0xDD77
void LD8BitRegToAddrsOfIYd( uint8_t &reg, uint8_t disp ){
	writeByte( indexIY + disp, reg );	
}

// LD (HL),n
// Copy integer value in 8-bit register into address at HL 
// OpCodes: 0x36
void LD8BitIntToAddrsOfHL( uint8_t val ){
	// Read the value of the PC to determine which operation we are doing
	writeByte(byteToWord(&regH, &regL), val);
}

// LD (IX+d),n
// Copy integer value in 8-bit register to address in IX+d
// OpCodes: 0xDD36
void LD8BitIntToAddrsOfIXd( uint8_t disp, uint8_t val ){
	writeByte( indexIX + disp, val );
}

// LD (IY+d),n
// Copy integer value in 8-bit register to address in IY+d 
// OpCodes: 0xFD36
void LD8BitIntToAddrsOfIYd( uint8_t disp, uint8_t val ){
	writeByte( indexIY + disp, val );
}

// LD A,(BC)
// Copy value in address of BC into 8-bit register A
// OpCodes: 0x0A
void LDAddrsOfBCToA(){
	regA = readByte( byteToWord(&regB, &regC) );
}

// LD A,(DE)
// Copy value in address of BC into 8-bit register A
// OpCodes: 0x1A
void LDAddrsOfDEToA(){
	regA = readByte( byteToWord(&regD, &regE) );
}

// LD A,(nn)
// Copy value in 16-bit integer address to 8-bit register A **Write to PORT**
// OpCodes: 0x3A
void LDAddrsOf16BitIntToA( uint8_t &reg, uint8_t srcConstHO, uint8_t srcConstLO ){
	reg = readByte( byteToWord(&srcConstHO, &srcConstLO) );
}

// LD (BC),A
// Copy value in 8-bit register A to address in BC
// OpCodes: 0x02
void LDAToAddrsOfBC(){
	regA = readByte( byteToWord( &regB, &regC )  );
}

// LD (DE),A
// Copy value in 8-bit register A to address in DE
// OpCodes: 0x12
void LDAToAddrsOfDE(){
	regA = readByte( byteToWord( &regD, &regE )  );
}

// LD (nn),A
// Copy 16-bit integer value at 16-bit register address to 8-bit register 
// OpCodes: 0x32
void LDAToAddrsOf16BitInt( uint8_t dstIntHO, uint8_t dstIntLO ){
	writeByte( byteToWord(&dstIntHO, &dstIntLO), regA );
}

// LD A,I 
// Copy value in 8-bit register I to 8-bit register A, and alter flags
// OpCodes: 0xED57
void LDIToA(){

	// Load in interrupt vector
	regA = regI;

	// *** NEED TO SET FLAGS HERE *** NEEDS TESTING

	// Check I if S is 0(positive) set the flag to 0 else if 1(negative) set S to 1 
	if( getBit( regI, 7 ) ){
		setBit( regF, 7, 1);
	}else{
		setBit( regF, 7, 0);
	}

	// Check I if zero, Z is 1 else 0
	if( regI == 0x00 ){
		setBit( regF, 6, 1);
	}else{
		setBit( regF, 6, 0);
	}

	// H is always set to 0
	setBit( regF, 3, 0);

	// Set P/V to IFF2 **Set to 0 is interrupt happends during execution of instruction**
	setBit( regF, 2, IFF2);

	// N is set to 0
	setBit( regF, 1, 0);
}

// LD A,R
// Copy value in 8-bit register R to 8-bit register A, and alter flags
// OpCodes: 0xED5F
void LDRToA(){
	
	regA = regR;

	// *** NEED TO SET FLAGS HERE ***

	// Check I if S is 0(positive) set the flag to 0 else if 1(negative) set S to 1 
	if( getBit( regR, 7 ) ){
		setBit( regF, 7, 1 );
	}else{
		setBit( regF, 7, 0 );
	}

	// Check I if zero, Z is 1 else 0
	if( regR == 0x00 ){
		setBit( regF, 6, 1 );
	}else{
		setBit( regF, 6, 0 );
	}

	// H is always set to 0
	setBit( regF, 4, 0 );

	// Set P/V to IFF2 **Set to 0 is interrupt happends during execution of instruction**
	setBit( regF, 2, IFF2 );

	// N is set to 0
	setBit( regF, 1, 0 );
}

// LD I,A
// Copy value in 8-bit register I to 8-bit register A
// OpCodes: 0xED47
void LDAToI(){
	regI = regA;
}

// LD R,A
// Copy value in 8-bit register R to 8-bit register A
// OpCodes: 0xED4F
void LDAToR(){
	regR = regA;
}
