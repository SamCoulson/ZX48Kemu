#include "../include/util_bit_operations.h"

// Set the given bit in a byte to either 1 or 0
void setBit(uint8_t *byte, uint8_t pos, uint8_t bitval){
	
	if( (pos < 0 ) && ( pos > 7 ) ){
		printf( "invalid bit range passed to setBit" );
		return;
	}

	if( bitval < 0 || bitval > 1 ){
		printf( "invalid bit value passed to setBit" );
		return;
	}

	if( bitval == 0 )
		*byte &= ~( 1 << ( pos ) ); 
		// Set to 0, 00000001 shifted to the position of pos, 
		// AND i.e. both need to be true to set to 1, 
		// then inverse the result with ~
	else
		*byte |= ( 1 << ( pos ) ); 
		// Set to 1, 00000001 shifted to the position of pos, 
		// OR i.e. either 0 or 1 will set to 1
}

// Find the status of a specific bit **TEST THIS**
int getBit( uint8_t *byte, uint8_t pos ){

	if( (pos < 0 ) && (pos > 7) ){
		printf( "invalid bit range passed to setBit\n" );
		return 0;
	}

	// AND with a 1 at corresponding position along the bit string
	if( *byte & ( 1 << ( pos ) ) ){
		return 1; // If non-zero return 1
	}else{
		return 0; // If zero return 0
	}
}
