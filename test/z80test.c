// Unit tests for z80 emu

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include "../include/cpu.h"
#include "../include/util_bit_operations.h"
#include "../include/8-bit_arithmetic_group.h"

// Declare and initialise a register set for testing
static Registers registers = {
        // Assign const pointers to main registers	
	.a = &af._af[1], .f = &af._af[0], .af = &af.af,
	.b = &bc._bc[1], .c = &bc._bc[0], .bc = &bc.bc,
	.d = &de._de[1], .e = &de._de[0], .de = &de.de,
	.h = &hl._hl[1], .l = &hl._hl[0], .hl = &hl.hl,
	// Assign const pointers to alternative registers 
	.alta = &altaf._af[1], .altf = &altaf._af[0], .altaf = &altaf.af,
	.altb = &altbc._bc[1], .altc = &altbc._bc[0], .altbc = &altbc.bc,
	.altd = &altde._de[1], .alte = &altde._de[0], .altde = &altde.de,
	.alth = &althl._hl[1], .altl = &althl._hl[0], .althl = &althl.hl,
	// 8-bit Special registers - interrupt vector
	.i = &ir._ir[1], .r = &ir._ir[0], .ir = &ir.ir,
	// 16-bit index registers
	.ix = &ix, .iy = &iy,
      	// 16-bit stack pointer and program pointer
	.sp = &sp, .pc = &pc,
	// IFF (Interrupt enabled flip-flop)
	.iff1 = &iff1, .iff2 = &iff2 	
};

// Pointer to the register structure
Registers* reg = &registers;

// Allows the CPU registers to be zero'd out
int resetCPU(){
	// Initialise registers to starting values, this can be used for testing
	*reg->af = 0x0000;
	*reg->bc = 0x0000;
	*reg->de = 0x0000;
	*reg->hl = 0x0000;

	*reg->altaf = 0x0000;
	*reg->altbc = 0x0000;
	*reg->altde = 0x0000;
	*reg->althl = 0x0000;

	*reg->ir = 0x00;
	*reg->ix = 0x00;
	*reg->iy = 0x00;

	*reg->sp = 0x00;
	*reg->pc = 0x00;

	*reg->iff1 = 0x00;
	*reg->iff2 = 0x00;

	return 0;
}

static void test_bit_manipulation(){
	
	// Test Bit 
	resetCPU();
	// Signed bit is 0
	uint8_t foo = 0x0;
	printf("Signed bit = %d\n", (foo & ( 1 << 7 ) ) ); 
	foo |= ( 1 << 7 );
	printf("Signed bit = %d\n", (foo & ( 1 << 7 ) ) );
		
}

static void test_8bit_add(){
	
	// Test Carry flag
	resetCPU();
	*reg->a = 128;
	*reg->b = 128;
	ADD( reg->a, reg->b, reg->f );
	assert( *reg->a == 0 && getBit( reg->f, 0) == 0x01  && "test_8bit_add() - Carry Flag" );

	// Test N(Addition/Subtraction) flag
	resetCPU();
	*reg->a = 1;
	*reg->b = 1;
	ADD( reg->a, reg->b, reg->f );
	assert( *reg->a == 2 && getBit( reg->f, 1 ) == 0x00  && "test_8bit_add() - N Flag" );

	// Test P/V flag (parity / overflow)
	resetCPU();
	*reg->a = 64;
	*reg->b = 64;
	ADD( reg->a, reg->b, reg->f );
	assert( *reg->a == 128 && getBit( reg->f, 2 ) == 0x01  && "test_8bit_add() - P/V Flag" );
	
	// Test H (Half carry) flag
	//resetCPU();
	//*reg->a = 4;
	//*reg->b = 4;
	//ADD( reg->a, reg->b, reg->f );
	//assert( *reg->a == 8 && getBit( reg->f, 4 ) == 0x01  && "test_8bit_add() - H Flag" );
	
	const uint8_t values[6] = { 0, 1, 127, 128, 129, 255 };

	resetCPU();
	for( int i = 0; i < sizeof( values ); i++){

		*reg->a = values[i]; 
		printf("Uint Sint  Uint Sint\n");	

		for( int j = 0; j < sizeof( values ); j++ ){
			
			*reg->b = values[j];
			
			printf("%3d(%4d) + %3d(%4d) = ", *reg->a, (int8_t)*reg->a, *reg->b, (int8_t)*reg->b ); 
			
			ADD( reg->a, reg->b, reg->f );
		
			printf(" %3d(%4d) C = %d V = %d\n", *reg->a, (int8_t)*reg->a, getBit( reg->f, 0 ),
			     getBit( reg->f, 2 ) );
			// Reset a
			*reg->a = values[i];
		}

	}
}

int main( int argc, char* argv[] ){

	printf("Testing started!\n");
	//test_bit_manipulation();

	printf("Testing 8Bit ADD\n");
	test_8bit_add();
	printf("Testing 8Bit ADD successful\n");

	printf( "Testing successful" );

	return 0;
}
