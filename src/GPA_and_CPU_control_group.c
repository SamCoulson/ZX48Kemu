#include "../include/GPA_and_CPU_control_group.h"
#include "../include/util_bit_operations.h"
// ***General-Purpose Arithmetic and CPU Control Group***
	
// DAA
// Conditionally adjust the accumulator for BCD conditions
// OpCodes: 0x27

// CPL
// Contents of accumulator A are inverted to the complement
// OpCodes: 0x2F
void CPL( uint8_t* aReg, uint8_t* fReg ){
	
	// Invert the value in a register
	*aReg = ~(*aReg);

	// H is set
	setBit( fReg, 4, 1 );

	// N is set
	setBit( fReg, 1, 1 );
}	

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
void NOP(Z80* z80)
{
	// Suposed to wait 6 T-cycles
}

// HALT
// Suspend CPU operation until an interupt is received
// OpCodes: 0x76

// DI
// Disable the maskable interrupts IFF1 and IFF2 by resetting enable interupt flip-flop
// OpCodes: 0xF3
void DI( Z80* z80 )
{	// Disable maskable interrupt so that maskable interrupt requests are ignored` 
	*z80->iff1 = 0;
	*z80->iff2 = 0;	
}

// EI
// Enable interupt instruction sets, sets both IFF1 and IFF2.
// OpCodes: 0xFB
void EI( int* iff1, int* iff2 ){
	*iff1 = 1;
	*iff2 = 1;

	// May need to set interrupt count down back one as the next instruction after an EI needs to be allowed
	// to execute because it might be a return instruction.
}	

// IM 0
// Set interrupt mode 0
// OpCodes: 0xED46
void IM0( int* mode ){
	*mode = 0;
}

// IM 1
// Set interrupt mode 1
// opCodes: 0xED56
void IM1( int* mode ){
	// responds to an interrupt by execuing restart to 0038h
	*mode = 1;
}

// IM 2
// Set interrupt mode 2
// OpCodes: 0xED5E
void IM2( int* mode ){
	*mode = 2;
}

