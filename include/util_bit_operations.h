#ifndef INC_UTILBITOPERATIONS_H
#define INC_UTILBITOPERATIONS_H

#include <stdio.h>
#include <stdint.h>

// Set the given bit in a byte to either 1 or 0
void setBit(uint8_t *byte, uint8_t pos, uint8_t bitval);

int getBit( uint8_t *byte, uint8_t pos );

#endif //INC_UTILBITOPERATIONS_H  
