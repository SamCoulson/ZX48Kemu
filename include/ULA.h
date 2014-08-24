#ifndef INC_ULA_H
#define INC_ULA_H

#include <stdint.h>
// ULA emulation

// Read data from port takes a pointer to the ports array on CPU
void initULA();
void readKeys();
void readVideoRAM( unsigned char* mem );

#endif // INC_ULA_H 
