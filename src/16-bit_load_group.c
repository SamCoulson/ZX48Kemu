#include "../include/cpu.h"

// LOAD data methods for 16-bit groups

// *** 16-BIT LOAD GROUP ***

// LD dd,nn
// Copy 2-byte integer value into 16-bit register 
// OpCodes: 0x01, 0x11, 0x21, 0x31
void LD16BitConstTo16BitReg( uint8_t *dstHO, uint8_t *dstLO, uint8_t srcHO, uint8_t srcLO ){
	dstHO = srcHO;
	dstLO = srcLO;	
}

// LD IX,nn
// Copy 2-byte integer value into IX 16-bit register
// OpCodes: 0xDD21
void LD16BitIntToIXReg( uint8_t srcHO, uint8_t srcLO ){
	uint16_t word = byteToWord( &srcHO, &srcLO );
	indexIX = word;
}
// LD IY,nn
// Copy 2-byte integer value into IY 16-bit register
// OpCodes: 0xFD21
void LD16BitIntToIYReg( uint8_t srcHO, uint8_t srcLO ){
	uint16_t word = byteToWord( &srcHO, &srcLO );
	indexIX = word;
}

// LD HL,(nn)
// Copy value into 16-bit HL register at the 16-bit integer address 
// OpCodes: 0x2A
void LDAddrsOf16BitIntToHLReg( uint8_t srcIntHO, uint8_t srcIntLO ){		
	regL = readByte( byteToWord( &srcIntHO, &srcIntLO ) );
	regH = readByte( byteToWord( &srcIntHO, &srcIntLO )+1 );
}

// LD dd,(nn)
// Copy value at 16-bit integer address to 16-bit register
// OpCodes: 0xED4B 
void LDAddrsOf16BitIntTo16BitReg( uint8_t *regHO, uint8_t *regLO, uint8_t srcIntHO, uint8_t srcIntLO ){
	// The LO register is filled with the value at (nn), the HO with value at (nn+1)
	
	regLO = readByte( byteToWord( &srcIntHO, &srcIntLO ) );
	regHO = readByte( byteToWord( &srcIntHO, &srcIntLO )+1 );
}



// LD IX,(nn)
// Copy value into 16-bit HL IX register at the 16-bit integer address
// OpCodes: 0xDD2A
void LD16BitAddrsOf16BitIntToIX( uint8_t srcIntHO, uint8_t srcIntLO ){
	
	// Get 16-bit value from source address
	uint16_t srcWord = byteToWord( &srcIntHO, &srcIntLO );

	// Read first byte at memory address
	uint8_t LObyte = readByte( srcWord );

	// Increment 16-bit address location to next byte
	srcWord++;

	// Read second byte at memory address
	uint8_t HObyte = readByte( srcWord );

	// Write the both bytes to the 16-bit register IX register
	indexIX = byteToWord( &HObyte, &LObyte );		
}

// LD IY,(nn)
// Copy value into 16-bit HL IY register at the 16-bit integer address
// OpCodes: 0xFD2A
void LD16BitAddrsOf16BitIntToIY( uint8_t srcIntHO, uint8_t srcIntLO ){

	// Get 16-bit value from source address
	uint16_t srcWord = byteToWord( &srcIntHO, &srcIntLO );

	// Read first byte at memory address
	uint8_t LObyte = readByte( srcWord );

	// Increment 16-bit address location to next byte
	srcWord++;

	// Read second byte at memory address
	uint8_t HObyte = readByte( srcWord );

	// Write the both bytes to the 16-bit register IX register
	indexIX = byteToWord( &HObyte, &LObyte );	
}

// LD(nn),HL
// Copy 16-bit HL register value into 16-bit integer address 
// OpCodes: 0x22
void LDHLRegToAddrsOf16BitInt( uint8_t dstIntHO, uint8_t dstIntLO ){
	LD16BitRegToAddrsOf16BitInt( &dstIntHO, &dstIntLO, regH, regL );
}

// LD(nn),dd
// Copy 16-bit register value into 16-bit integer address
// OpCodes: 0xED43, 0xED53
void LD16BitRegToAddrsOf16BitInt( uint8_t dstIntHO, uint8_t dstIntLO, uint8_t *srcRegHO, uint8_t *srcRegLO ){

	// Copy value in LO reg in to address nn
	writeByte( byteToWord( &dstIntHO, &dstIntLO), srcRegLO );

	// Copy value in HO reg in to address nn+1
	writeByte( byteToWord( &dstIntHO, &dstIntLO)+1, srcRegHO );
}

// LD(nn),IX
// Copy 16-bit IX register value into 16-bit integer address
// OpCodes: 0xDD22
void LDIXRegToAddrsOf16BitInt( uint8_t dstIntHO, uint8_t dstIntLO ){
	writeByte( byteToWord( &dstIntHO, &dstIntLO ), getLOByte( &indexIX ) );
	writeByte( byteToWord( &dstIntHO, &dstIntLO )+1, getLOByte( &indexIX ) );
}

// LD(nn),IY
// Copy 16-bit IY register value into 16-bit integer address
// OpCodes: 0xFD22
void LDIYRegToAddrsOf16BitInt( uint8_t dstIntHO, uint8_t dstIntLO ){
	writeByte( byteToWord( &dstIntHO, &dstIntLO ), getLOByte( &indexIY ) );
	writeByte( byteToWord( &dstIntHO, &dstIntLO )+1, getLOByte( &indexIY ) );
}

// LD SP,HL
// Copy 16-bit HL register value into SP 16-bit register
// OpCodes: 0xF9
void LDHLRegToSPReg(){
	sp = byteToWord( &regH, &regL );
}

// LD SP,IX
// Copy 16-bit IX register value into SP 16-bit register
// OpCodes: 0xDDF9
void LDIXRegToSPReg(){
	sp = indexIX;

}

// LD SP,IY
// Copy 16-bit IY register value into SP 16-bit register
// OpCodes: 0xFDF9
void LDIYRegToSPReg(){
	sp = indexIY;

}

// PUSH qq
// Copy 16-bit register value onto the top of the stack
// OpCodes: 0xC5, 0xD5, 0xE5 0xF5
void PUSH( uint8_t *regHO, uint8_t *regLO ){
	// Decrement the stack pointer
	sp--;

	// Write the HO reg value into the address specified by SP
	writeByte( sp, regHO );
	
	// Decrement stack pointer again
	sp--;

	// Write the LO reg value into the address specified by SP
	writeByte( sp, regLO );
	
}

// PUSH IX
// Copy 16-bit IX register value onto the top of the stack
// OpCodes: 0xDDE5
void PUSHIX(){
	// Decrement the stack pointer
	sp--;

	// Write the HO reg value into the address specified by SP
	writeByte( sp, getHOByte( &indexIX ) );

	// Decrement stack pointer again
	sp--;

	// Write the LO reg value into the address specified by SP
	writeByte( sp, getLOByte( &indexIX ) );
		

}

// PUSH IY
// Copy 16-bit IY register value onto the top of the stack
// OpCodes: 0xFDE5
void PUSHIY(){
	// Decrement the stack pointer
	sp--;

	// Write the HO reg value into the address specified by SP
	writeByte( sp, getHOByte( &indexIY ) );

	// Decrement stack pointer again
	sp--;

	// Write the LO reg value into the address specified by SP
	writeByte( sp, getLOByte( &indexIY ) );
		
	
}

// POP qq
// Copy 2-bytes from top of stack in a 16-bit register
// OpCodes: 0xC1, 0xD1, 0xE1 0xF1
void POP( uint8_t *regHO, uint8_t *regLO ){
	// Copy value at current stack pointer to LO of reg
	regLO = sp;

	// Increment the stack pointer
	sp++;
	
	// Copy value at new stack pointer
	regHO = sp;	


}

// POP IX
// Copy 2-bytes from top of stack in a 16-bit IX register
// OpCodes: 0xDDE1
void POPIX(){
	// Store value at current stack pointer 
	uint8_t LObyte = sp;

	// Increment the stack pointer
	sp++;
	
	// Store value at new stack pointer
	uint8_t HObyte = sp;	

	// Combine LO and Ho byte in to a word and copy to IX
	indexIX = byteToWord( &HObyte, &LObyte );
}

// POP IY
// Copy 2-bytes from top of stack in a 16-bit IY register
// OpCodes: 0xFDE1
void POPIY(){
	// Store value at current stack pointer 
	uint8_t LObyte = sp;

	// Increment the stack pointer
	sp++;
	
	// Store value at new stack pointer
	uint8_t HObyte = sp;	

	// Combine LO and Ho byte in to a word and copy to IX
	indexIY = byteToWord( &HObyte, &LObyte );
}	

