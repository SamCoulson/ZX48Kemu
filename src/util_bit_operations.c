#include "../include/util_bit_operations.h"

// Set the given bit in a byte to either 1 or 0
void setBit(uint8_t *byte, uint8_t pos, uint8_t bitval){
	
	if( bitval == 0 )
		*byte &= ~( 1 << ( pos ) ); 
		// Set to 0, 00000001 shifted to the position of pos, 
		// AND i.e. both need to be true to set to 1, 
		// then inverse the result with NOT~
	else
		*byte |= ( 1 << ( pos ) ); 
		// Set to 1, 00000001 shifted to the position of pos, 
		// OR i.e. either 0 or 1 will set to 1
}

// Find the status of a specific bit **TEST THIS**
int getBit( uint8_t *byte, uint8_t pos ){

	// AND with a 1 at corresponding position along the bit string
	if( *byte & ( 1 << ( pos ) ) ){
		return 1; // If non-zero return 1
	}else{
		return 0; // If zero return 0
	}
}

// Taken from http://bytes.com/topic/c/answers/705467-byte-parity-question
// neat trick to return 1 as odd and 0 as even
int byte_parity( unsigned char b ){
	b ^= b >4;
	b ^= b >2;
	b ^= b >1;
	return b & 1;
}
