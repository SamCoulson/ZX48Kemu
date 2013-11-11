#include "../include/CPU.h"
#include "../include/util_bit_operations.h"

// ***EXCHANGE, BLOCK TRANSFER AND SEARCH GROUPS***

// EX DE,HL
// Swap DE and HL register values
// OpCodes: 0xEB

// EX AF,AF'
// Swap register values between AF and alternative AF 
// OpCodes: 0x08

// EX (SP),HL
// Swap values between two HL register and 2-bytes on stack at SP
// OpCodes: 0xE3

// EX (SP),IX
// Swap values between two HL register and 2-bytes on stack at SP
// OpCodes: 0xDDE3

// EX (SP),IY
// Swap values between two IX register and 2-bytes on stack at SP 
// OpCodes: 0xFDE3
void EX( uint16_t* val1, uint16_t* val2 ){
	uint16_t tmp;

	tmp = *val1;
	*val1 = *val2;
	*val2 = tmp;
}

// EXX
// Swap register BC & BC', DE & DE', and HL & HL'
// OpCodes: 0xD9
void EXX( uint16_t* bc, uint16_t* de, uint16_t* hl, uint16_t* altbc, uint16_t* altde, uint16_t* althl ){
	
	uint16_t tmp;

	// Swap BC with alt BC
	tmp = *bc;
	*bc = *altbc;
	*altbc = tmp;
	tmp = 0x0000;
	// Swap DE with alt DE
	tmp = *de;
	*de = *altde;
	*altde = tmp;	
	tmp = 0x0000;
	// Swap HL with alt HL
	tmp = *hl;
	*hl = *althl;
	*althl = tmp;
}

/*
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

	// H flag reset
	setBit(regF, 4, 0 );

	// P/V flag
	// if BC-1 != 0, set 1 else 0
	if( (byteToWord( &regB, &regC)-1) != 0 ){
		setBit(regF, 2, 1 );
	}else{
		setBit(regF, 2, 0 );
	}

	// N flag rest
	setBit(regF, 4, 0 );
}
*/
// LDIR *Changes flags*
// Copy value at address in HL register to address in DE register
// Increment both HL and BC, and decrement BC
// OpCodes: 0xEDB0
void LDIR( uint16_t* hlVal, uint16_t* deVal, uint16_t* hl, uint16_t* de, uint16_t* bc, uint8_t* fReg ){
	
	// If BC = 0x00 set to 0xFA00 (64) so it can loop over again of 64K
	if( *bc == 0x0000){
		*bc = 0xFA00;
	}	
	
	// Keep copying data from one memory loaction to another until BC is 0 
	while( *bc != 0x0000 ){
		// Transfer byte pointed to by HL to location pointed to by BC
		*deVal = *hlVal;			

		// Increment HL and DE
		++*hl;
		++*de;	

		// Decrement BC
		--*bc;
	}

	// Reset flags H, P/V, and N
	setBit( fReg, 4, 0 );
	setBit( fReg, 2, 0 );
	setBit( fReg, 1, 0 );
}
/*
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
	setBit(regF, 4, 0 );

	// P/V flag
	// if BC-1 != 0, set 1 else 0
	if( (byteToWord( &regB, &regC)-1) != 0 ){
		setBit( regF, 2, 1 );
	}else{
		setBit( regF, 2, 0 );
	}

	// N flag reset
	setBit(regF, 4, 0 );
}
*/
// LDDR
// Copy value at adress in HL register to address in DE register
// Decrement both HL and BC, and decrement BC
// OpCodes: 0xEDB8
void LDDR( uint16_t* hlVal, uint16_t* deVal, uint16_t* hl, uint16_t* de, uint16_t* bc, uint8_t* fReg ){
	
	// If BC = 0x00 set to 0xFA00 (64) so it can loop over again of 64K
	if( *bc == 0x0000){
		*bc = 0xFA00;
	}	
	
	// Keep copying data from one memory loaction to another until BC is 0 
	while( *bc != 0x0000 ){
		// Transfer byte pointed to by HL to location pointed to by BC
		*deVal = *hlVal;			

		// Decrement HL and DE
		--*hl;
		--*de;	

		// Decrement BC
		--*bc;
	}

	// Reset flags H, P/V, and N
	setBit( fReg, 4, 0 );
	setBit( fReg, 2, 0 );
	setBit( fReg, 1, 0 );
}/*

// CPI *Changes flags*
// Contents of HL compared with contents of A
// OpCodes: 0xEDA1
void CPU::CPI(){
	if( readByte( byteToWord( &regH, &regL ) ) == regA ){
		setBit( regF, 7, 0 ); // Reset S
		setBit( regF, 6, 1 ); // Set Z
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
		setBit( regF, 7, 1 );
		setBit( regF, 6, 0 );
		//See above for H flag		
	}

	// P/V flag
	// if BC-1 != 0, set 1 else 0
	if( (byteToWord( &regB, &regC)-1) != 0 ){
		setBit(regF, 2, 1 );
	}else{
		setBit(regF, 2, 0 );
	}

	setBit( regF, 1, 1 ); // N is set
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
		setBit( regF, 7, 0 ); // Reset S
		setBit( regF, 6, 1 ); // Set Z
		// setBitInByte( regF, ?, ? ); // Set H bit to borrow in bit 4
		// don't understand this
	}else{
		setBit( regF, 7, 1 );
		setBit( regF, 6, 0 );
			//See above for H flag		
	}
			

	// P/V flag
	// if BC-1 != 0, set 1 else 0
	if( (byteToWord( &regB, &regC)-1) != 0 ){
		setBit(regF, 2, 1 );
	}else{
		setBit(regF, 2, 0 );
	}

	setBit( regF, 1, 1 ); // N is set	
}

// CPD
// Contents of HL compared with contents of A
// HL and BC affetced by outcome of comparison 
// OpCodes: 0xEDA9
void CPU::CPD(){
	if( readByte( byteToWord( &regH, &regL ) ) == regA ){
		setBit( regF, 7, 0 ); // Reset S
		setBit( regF, 6, 1 ); // Set Z
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
		setBit( regF, 7, 1 );
		setBit( regF, 6, 0 );
		//See above for H flag		
	}

	// P/V flag
	// if BC-1 != 0, set 1 else 0
	if( (byteToWord( &regB, &regC)-1) != 0 ){
		setBit(regF, 2, 1 );
	}else{
		setBit(regF, 2, 0 );
	}

	setBit( regF, 1, 1 ); // N is set
	
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
		setBit( regF, 7, 0 ); // Reset S
		setBit( regF, 6, 1 ); // Set Z
		// setBitInByte( regF, ?, ? ); // Set H bit to borrow in bit 4
		// don't understand this
	}else{
		setBit( regF, 7, 1 );
		setBit( regF, 6, 0 );
			//See above for H flag		
	}
			

	// P/V flag
	// if BC-1 != 0, set 1 else 0
	if( (byteToWord( &regB, &regC)-1) != 0 ){
		setBit(regF, 2, 1 );
	}else{
		setBit(regF, 2, 0 );
	}

	setBit( regF, 1, 1 ); // N is set
}
*/
