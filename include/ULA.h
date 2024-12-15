#ifndef INC_ULA_H
#define INC_ULA_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
// ULA emulation

// Read data from port takes a pointer to the ports array on CPU
void initULA();
void readKeys();
void readVideoRAM( unsigned char* mem );
bool shouldUpdateScreen;
#endif // INC_ULA_H 
