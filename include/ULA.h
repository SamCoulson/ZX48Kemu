#ifndef INC_ULA_H
#define INC_ULA_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "../include/memory.h"
#include "../include/screen.h"
#include "../include/keyboard.h"
#include "../include/cpu.h"
// Read data from port takes a pointer to the ports array on CPU

void initULA();
void readKeys();
void readVideoRAM( unsigned char* mem );
extern bool shouldUpdateScreen;

#endif // INC_ULA_H 
