#include "../include/GPA_and_CPU_control_group.h"
#include "../include/util_bit_operations.h"
// ***General-Purpose Arithmetic and CPU Control Group***
	
// DAA
// Conditionally adjust the accumulator for BCD conditions
// OpCodes: 0x27

// CPL
// Contents of accumulator A are inverted to the complement
// OpCodes: 0x2F 

// NEG
// The contentsof the accumulator is negated (subtracted from 0)
// OpCodes: 0xED44

// CCF
// The carry flag in the F register is negated
// OpCodes: 0x3F
void CCF( uint8_t* fReg ){
	if( getBit( fReg, 0 ) == 0 ){
		setBit( fReg, 0, 1 );
	}else{
		setBit( fReg, 0, 0 );
	}
}

// SCF
// The carry flag in the F register is set
// OpCodes: 0x37
void SCF( uint8_t* fReg ){
	// H Reset
	setBit( fReg, 4, 0 );

	// N Reset
	setBit( fReg, 1, 0 );
	
	// C is Set
	setBit( fReg, 0, 1 );
}

// NOP
// Perform no operation during the machine cycle
// OpCodes: 0x00
void NOP(){
	// Suposed to wait 6 T-cycles
}

// HALT
// Suspend CPU operation until an interupt is received
// OpCodes: 0x76

// DI
// Disable the maskable interrupts IFF1 and IFF2 by resetting enable interupt flip-flop
// OpCodes: 0xF3
void DI( int* iff1, int* iff2 ){
	// Disable maskable interrupt so that maskable interrupt requests are ignored` 
	*iff1 = 0;
	*iff2 = 0;	
}

// EI
// Enable interupt instruction sets, sets both IFF1 and IFF2.
// OpCodes: 0xFB
void EI( int* iff1, int* iff2 ){
	*iff1 = 1;
	*iff2 = 1;
}	

// IM 0
// Set interrupt mode 0
// OpCodes: 0xED46

// IM 1
// Set interrupt mode 1
// opCodes: 0xED56
void IM1( int* iff1 ){
	*iff1 = 1;
}

// IM 2
// Set interrupt mode 2
// OpCodes: 0xED5E

