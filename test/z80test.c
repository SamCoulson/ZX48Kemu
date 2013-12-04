// Unit tests for z80 emu

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include "../include/util_bit_operations.h"
#include "../include/8-bit_arithmetic_group.h"

static void test_8bit_add(){
	
	uint8_t i = 0xff;
	uint8_t j = 0xff;

	uint8_t f = 0;

	uint8_t* p_i = &i;
	uint8_t* p_j = &j;

	uint8_t* p_f = &f;

	ADD( p_j, p_i, p_f );

	printf("Value = %d\n",*p_j);
	assert( *p_j < 256 && "test_8bit_add()" );
}

int main( int argc, char* argv[] ){

	printf("Testing started!\n");

	test_8bit_add();

	printf( "Testing finished" );

	return 0;
}
