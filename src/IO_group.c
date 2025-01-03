#include "../include/IO_group.h"
//#include "../include/ULA.h"
#include "../include/util_bit_operations.h"
#include <stdio.h>

// *** Input and Output group ***

// IN A,(n)
// Read in one byte from IO mapped port (n) into register A 
// OpCodes: 0xDB
void INA( uint8_t *reg, uint8_t port ){
	printf( "INA sees %X at port 0xFE", port );
	*reg = port; 
}

// IN r,(C)
// IO ported is selected through contents of register C which selects one of 256 ports, register B forms the 
// second part a byte copied from that IO device address in to register r
//
// NOTE: ** Although the instruction in the manual does not give it away register B is also important here **
// 	    as it can be used to address locations within the port specified in C.
//
// OpCodes: 0xED40, 0xED48, 0xED50, 0xED58, 0xED60, 0xED68, 0xED78 
void IN( uint8_t *reg, uint8_t *bReg, uint8_t portVal, uint8_t *fReg ){
	
	// In the 
	*reg = portVal;
	
	printf( "IN A,(C) sees %X at port 0x%XFE", portVal, *bReg );

	// S is set if input data is negative
	if( getBit( &portVal, 7 ) == 0x01 ){
		setBit( fReg, 7, 1 );
	}else{
		setBit( fReg, 7, 0 );
	}

	// Z is set if input data is 0 otherwise reset
	if( portVal == 0x00 ){
		setBit( fReg, 6, 1 );
	}else{
		setBit( fReg, 6, 0 );
	}

	// H is reset
	setBit( fReg, 4, 0 );

	// P/V is set if parity is even
	
	// N is reset
	setBit( fReg, 1, 0 );

}


// INI
// IO port is selected by the contents of register C, the byte is 
// read in and placed into the contents of the HL register, the byte
// counter is decremented and the HL incremented
// OpCodes: 0xEDA2
void INI();

// INIR
// IO port is selected by the contents of register C, the byte is 
// read in and placed into the contents of the HL register, if B is non-zero
// repeat instruction
// OpCodes: 0xEDB2
void INIR();

// IND
// IO port is selected by the contents of register C, the byte is 
// read in and placed into the contents of the HL register, the byte
// counter is incremented and the HL incremented
// OpCodes: 0xEDAA
void IND();

// INDR
// IO port is selected by the contents of register C, the byte is 
// read in and placed into the contents of the HL register, if B is non-zero
// repeat instruction
// OpCodes: 0xEDBA
void INDR();	

// OUT (n),A
// The output port is selected with address of the integer along with 
// register A, the byte in register A is copied to the address
// OpCodes: 0xD3
void OUTA( uint8_t *port, uint8_t* val ){
	// Write value to the 0xFE "port"
	*port = *val;	
} 

// OUT (C),r
// Contents of register C is used to select an ouput port, byte in register
// r is copied to the port
// OpCodes: 0xED41, 0xED49, 0xED51, 0xED59, 0xED61, 0xED69, 0xED79
void OUTC( uint8_t *reg );

// OUTI
// Output to port referecne by contents of address in HL register
// register B is used as a counter and is decremented
// OpCodes: 0xEDA3
void OUTI();

// OTIR
// Output to port referecne by contents of address in HL register
// register B is used as a counter and is decremented, the instruction is 
// repeated if B is non-zero
// OpCodes: 0xEDB3
void OTIR();	

// OUTD
// Output to port referecne by contents of address in HL register
// register B is used as a counter and is decremented
// OpCodes: 0xEDAB
void OUTD();	

// OTDR
// Output to port referecne by contents of address in HL register
// register B is used as a counter and is decremented, the instruction is 
// repeated if B is non-zero
// OpCodes: 0xEDBB
void OTDR();
