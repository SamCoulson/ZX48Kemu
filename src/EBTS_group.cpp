#include "../include/CPU.h"

// ***EXCHANGE, BLOCK TRANSFER AND SEARCH GROUPS***

// EX DE,HL
// Swap DE and HL register values
// OpCodes: 0xEB
void CPU::EXDEHL(){

	uint8_t tmp = 0;

	// Swap D with H
	tmp = regD;
	regD = regH;
	regH = tmp;

	// Swap E with L
	tmp = regE;
	regE = regL;
	regL = tmp;

	pc++;
}

// EX AF,AF'
// Swap register values between AF and alternative AF 
// OpCodes: 0x08
void CPU::EXAFAltAF(){
	uint8_t tmp = 0;

	// Swap A with A'
	tmp = regA;
	regA = aregA;
	aregA = tmp;

	// Swap F with F'
	tmp = regE;
	regE = regL;
	regL = tmp;

	pc++;	
}

// EXX
// Swap register BC & BC', DE & DE', and HL & HL'
// OpCodes: 0xD9
void CPU::EXX(){
	uint8_t tmpBC, tmpDE, tmpHL;

	// Swap BC with alt BC
	tmpBC = regB;
	regB = aregB;
	aregB = tmpBC;

	tmpBC = regC;
	regC = aregC;
	aregC = tmpBC;

	// Swap DE with alt DE
	tmpDE = regD;
	regD = aregD;
	aregD = tmpDE;

	tmpDE = regE;
	regE = aregE;
	aregE = tmpDE;

	// Swap HL with alt HL
	tmpHL = regH;
	regH = aregH;
	aregH = tmpHL;

	tmpHL = regL;
	regL = aregL;
	aregL = tmpHL;

	pc++;
}

// EX (SP),HL
// Swap values between two HL register and 2-bytes on stack at SP
// OpCodes: 0xE3
void CPU::EXHLToAddrsOfSP(){
	
	uint8_t tmp = 0;
	
	// Exchange LO byte of HL to address in SP
	tmp = regL;
	regL = readByte( sp );
	writeByte( sp, tmp );
	
	// Exchange HO byte of HL to address in SP
	tmp = regH;
	regH = readByte( sp+1 );
	writeByte( sp+1, tmp );
	
	pc++;
}

// EX (SP),IX
// Swap values between two HL register and 2-bytes on stack at SP
// OpCodes: 0xDDE3
void CPU::EXIXToAddrsOfSP(){
	
	// Exchange LO byte of IX to address in SP

	// Store the HO and LO bytes seperately in indexIX
	uint8_t idxLObyte = getLOByte( &indexIX );
	uint8_t idxHObyte = getHOByte( &indexIX );

	// Store the 2 bytes as SP address
	uint8_t spByteLO = readByte( sp );
	uint8_t spByteHO = readByte( sp+1 );

	// Copy sp bytes to index
	indexIX = byteToWord( &spByteHO, &spByteLO );
	
	// Write the index values to the locations at SP
	writeByte( sp, idxLObyte );
	writeByte( sp+1, idxHObyte );

	pc++;
}

// EX (SP),IY
// Swap values between two IX register and 2-bytes on stack at SP 
// OpCodes: 0xFDE3
void CPU::EXIYToAddrsOfSP(){
	// Exchange LO byte of IY to address in SP

	// Store the HO and LO bytes seperately in indexIX
	uint8_t idxLObyte = getLOByte( &indexIY );
	uint8_t idxHObyte = getHOByte( &indexIY );

	// Store the 2 bytes as SP address
	uint8_t spByteLO = readByte( sp );
	uint8_t spByteHO = readByte( sp+1 );

	// Copy sp bytes to index
	indexIX = byteToWord( &spByteHO, &spByteLO );
	
	// Write the index values to the locations at SP
	writeByte( sp, idxLObyte );
	writeByte( sp+1, idxHObyte );

	pc++;
}

// LDI *Changes flags*
// Copy value at address in HL register to address in DE register
// Increment both HL and DE, and decrement BC
// OpCodes: 0xEDA0
void CPU::LDI(){
	// Copy value at the address stored in HL to the address stored in DE
	uint16_t hlAddrs = byteToWord( &regH, &regL );
	uint16_t deAddrs = byteToWord( &regD, &regE ); 	
	uint8_t hlVal = readByte( hlAddrs );
	writeByte( deAddrs, hlVal );

	// Increment HL and DE
	uint16_t word = byteToWord(&regH, &regL);
	word++;
	regH = getLOByte(&word);
	regL = getHOByte(&word);
	
	word = byteToWord(&regD, &regE);
	word++;
	regD = getLOByte(&word);
	regE = getHOByte(&word);

	// Decrement BC
	word = byteToWord(&regB, &regC);
	word--;
	regB = getLOByte(&word);
	regC = getHOByte(&word);

	// H flag rest
	setBitInByte(regF, 5, 0 );

	// P/V flag
	// if BC-1 != 0, set 1 else 0
	if( (byteToWord( &regB, &regC)-1) != 0 ){
		setBitInByte(regF, 3, 1 );
	}else{
		setBitInByte(regF, 3, 0 );
	}

	// N flag rest
	setBitInByte(regF, 5, 0 );
}

// LDIR *Changes flags*
// Copy value at adress in HL register to address in DE register
// Increment both HL and BC, and decrement BC
// OpCodes: 0xEDB0
void CPU::LDIR(){

	// Transfer byte pointed to by HL to location pointed to by BC
	uint8_t byte = readByte( byteToWord( &regH, &regL ) );
	writeByte( byteToWord( &regD, &regE ), byte );

	// If BC = 0x00 set to 0xFA00 (64) so it can loop over again of 64K
	if( byteToWord(&regB, &regC) == 0x0000){
		regB = 0xFA;
		regC = 0x00;
	}

	while( ( regB != 0x00 ) && ( regC != 0x00 ) ){
		// Decrement program counter by two
		pc-=2;

		// increment HL, DE, and BC until BC is 0
		uint16_t word = byteToWord(&regD, &regE);
		word++;
		regD = getLOByte(&word);
		regE = getHOByte(&word);

		word = byteToWord(&regH, &regL);
		word++;
		regH = (int)getLOByte(&word);
		regL = (int)getHOByte(&word);

		// Decrement BC
		word = byteToWord(&regB, &regC);
		word--;
		
		regB = (int)getLOByte(&word);
		regC = (int)getHOByte(&word);
	}

	// Reset flags H, P/V, and N
	setBitInByte( regF, 5, 0 );
	setBitInByte( regF, 3, 0 );
	setBitInByte( regF, 2, 0 );
}

// LDD *Changes flags* 
// Copy value at adress in HL register to address in DE register
// Decrement both HL and BC, and decrement BC
// OpCodes: 0xEDA8
void CPU::LDD(){
	// Copy value at the address stored in HL to the address stored in DE
	uint16_t hlAddrs = byteToWord( &regH, &regL );
	uint16_t deAddrs = byteToWord( &regD, &regE ); 	
	uint8_t hlVal = readByte( hlAddrs );
	writeByte( deAddrs, hlVal );

	// Decrement HL and DE
	uint16_t word = byteToWord(&regH, &regL);
	word--;
	regH = getLOByte(&word);
	regL = getHOByte(&word);
	
	word = byteToWord(&regD, &regE);
	word--;
	regD = getLOByte(&word);
	regE = getHOByte(&word);

	// Decrement BC
	word = byteToWord(&regB, &regC);
	word--;
	regB = getLOByte(&word);
	regC = getHOByte(&word);

	// H flag reset
	setBitInByte(regF, 5, 0 );

	// P/V flag
	// if BC-1 != 0, set 1 else 0
	if( (byteToWord( &regB, &regC)-1) != 0 ){
		setBitInByte(regF, 3, 1 );
	}else{
		setBitInByte(regF, 3, 0 );
	}

	// N flag reset
	setBitInByte(regF, 5, 0 );

	pc++;
}

// LDDR *Changes flags*
// Copy value at adress in HL register to address in DE register
// Decrement both HL and BC, and decrement BC
// OpCodes: 0xEDB8
void CPU::LDDR(){
	// If BC = 0x00 set to 0xFA00 (64) so it can loop over again of 64K
	if( byteToWord(&regB, &regC) == 0x00){
		regB = 0xFA;
		regC = 0x00;
	}

	while( byteToWord( &regB, &regC ) > 0x00 ){

		// Decrement program counter by two
		pc-=2;

		// Decrement HL, DE, and BC until BC is 0
		uint16_t word = byteToWord(&regD, &regE);
		word--;
		regD = getLOByte(&word);
		regE = getHOByte(&word);

		word = byteToWord(&regH, &regL);
		word--;
		regH = getLOByte(&word);
		regL = getHOByte(&word);

		// Decrement BC
		word = byteToWord(&regB, &regC);
		word--;
		regB = getLOByte(&word);
		regC = getHOByte(&word);
	}

	// Reset flags H, P/V, and N
	setBitInByte( regF, 5, 0 );
	setBitInByte( regF, 3, 0 );
	setBitInByte( regF, 2, 0 );
}

// CPI *Changes flags*
// Contents of HL compared with contents of A
// OpCodes: 0xEDA1
void CPU::CPI(){
	if( readByte( byteToWord( &regH, &regL ) ) == regA ){
		setBitInByte( regF, 8, 0 ); // Reset S
		setBitInByte( regF, 7, 1 ); // Set Z
		// setBitInByte( regF, ?, ? ); // Set H bit to borrow in bit 4
		// don't understand this
		
		// Increment HL
		uint16_t hlVal = byteToWord( &regH, &regL );
		hlVal++;
		regH = getHOByte( &hlVal );
		regL = getLOByte( &hlVal );

		// Decrement BC
		uint16_t bcVal = byteToWord( &regB, &regC );
		bcVal++;
		regH = getHOByte( &bcVal );
		regL = getLOByte( &bcVal );

	}else{
		setBitInByte( regF, 8, 1 );
		setBitInByte( regF, 7, 0 );
		//See above for H flag		
	}

	// P/V flag
	// if BC-1 != 0, set 1 else 0
	if( (byteToWord( &regB, &regC)-1) != 0 ){
		setBitInByte(regF, 3, 1 );
	}else{
		setBitInByte(regF, 3, 0 );
	}

	setBitInByte( regF, 2, 1 ); // N is set

	pc++;
}

// CPIR *Change flags*
// Contents of HL compared with contents of A
// HL and BC affetced by outcome of comparison
// OpCodes: 0xEDB1
void CPU::CPIR(){

	// ***THIS WHOLE FUNCTIONS NEEDS CORRECTING POSSIBLY **


	// If BC = 0x00 set to 0xFA00 (64) so it can loop over again of 64K
	if( byteToWord(&regB, &regC) == 0x00){
		regB = 0xFA;
		regC = 0x00;
	}


	while( ( byteToWord( &regB, &regC ) > 0x00 ) || (readByte( byteToWord( &regH, &regL ) ) != regA  ) ){

		// Decremnt program counter by 2
		pc-=2;

		
		// Increment HL
		uint16_t hlVal = byteToWord( &regH, &regL );
		hlVal++;
		regH = getHOByte( &hlVal );
		regL = getLOByte( &hlVal );

		// Decrement BC
		uint16_t bcVal = byteToWord( &regB, &regC );
		bcVal--;
		regH = getHOByte( &bcVal );
		regL = getLOByte( &bcVal );

	}

	if( readByte( byteToWord( &regH, &regL ) ) == regA ){
		setBitInByte( regF, 8, 0 ); // Reset S
		setBitInByte( regF, 7, 1 ); // Set Z
		// setBitInByte( regF, ?, ? ); // Set H bit to borrow in bit 4
		// don't understand this
	}else{
		setBitInByte( regF, 8, 1 );
		setBitInByte( regF, 7, 0 );
			//See above for H flag		
	}
			

	// P/V flag
	// if BC-1 != 0, set 1 else 0
	if( (byteToWord( &regB, &regC)-1) != 0 ){
		setBitInByte(regF, 3, 1 );
	}else{
		setBitInByte(regF, 3, 0 );
	}

	setBitInByte( regF, 2, 1 ); // N is set

	pc++;	
}

// CPD
// Contents of HL compared with contents of A
// HL and BC affetced by outcome of comparison 
// OpCodes: 0xEDA9
void CPU::CPD(){
	if( readByte( byteToWord( &regH, &regL ) ) == regA ){
		setBitInByte( regF, 8, 0 ); // Reset S
		setBitInByte( regF, 7, 1 ); // Set Z
		// setBitInByte( regF, ?, ? ); // Set H bit to borrow in bit 4
		// don't understand this
		
		// Increment HL
		uint16_t hlVal = byteToWord( &regH, &regL );
		hlVal--;
		regH = getHOByte( &hlVal );
		regL = getLOByte( &hlVal );

		// Decrement BC
		uint16_t bcVal = byteToWord( &regB, &regC );
		bcVal--;
		regH = getHOByte( &bcVal );
		regL = getLOByte( &bcVal );

	}else{
		setBitInByte( regF, 8, 1 );
		setBitInByte( regF, 7, 0 );
		//See above for H flag		
	}

	// P/V flag
	// if BC-1 != 0, set 1 else 0
	if( (byteToWord( &regB, &regC)-1) != 0 ){
		setBitInByte(regF, 3, 1 );
	}else{
		setBitInByte(regF, 3, 0 );
	}

	setBitInByte( regF, 2, 1 ); // N is set

	pc++;
	
}

// CPDR
// Contents of HL compared with contents of A
// HL and BC affetced by outcome of comparison 
// OpCodes: 0xEDB9
void CPU::CPDR(){
// ***THIS WHOLE FUNCTIONS NEEDS CORRECTING POSSIBLY **


	// If BC = 0x00 set to 0xFA00 (64) so it can loop over again of 64K
	if( byteToWord(&regB, &regC) == 0x00){
		regB = 0xFA;
		regC = 0x00;
	}


	while( ( byteToWord( &regB, &regC ) > 0x00 ) || (readByte( byteToWord( &regH, &regL ) ) != regA  ) ){

		// Decremnt program counter by 2
		pc-=2;

		
		// Decrement HL
		uint16_t hlVal = byteToWord( &regH, &regL );
		hlVal--;
		regH = getHOByte( &hlVal );
		regL = getLOByte( &hlVal );

		// Decrement BC
		uint16_t bcVal = byteToWord( &regB, &regC );
		bcVal--;
		regH = getHOByte( &bcVal );
		regL = getLOByte( &bcVal );

	}

	if( readByte( byteToWord( &regH, &regL ) ) == regA ){
		setBitInByte( regF, 8, 0 ); // Reset S
		setBitInByte( regF, 7, 1 ); // Set Z
		// setBitInByte( regF, ?, ? ); // Set H bit to borrow in bit 4
		// don't understand this
	}else{
		setBitInByte( regF, 8, 1 );
		setBitInByte( regF, 7, 0 );
			//See above for H flag		
	}
			

	// P/V flag
	// if BC-1 != 0, set 1 else 0
	if( (byteToWord( &regB, &regC)-1) != 0 ){
		setBitInByte(regF, 3, 1 );
	}else{
		setBitInByte(regF, 3, 0 );
	}

	setBitInByte( regF, 2, 1 ); // N is set

	pc++;
}

