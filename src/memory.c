#include "../include/memory.h"
#include <stdio.h>

unsigned char memory[65536];

// Copy the ROM file data into system memory
int loadROMintoMemory(unsigned char *src, unsigned int noBytes)
{

    if (src == NULL)
    {
        printf("Unable to load ROM data into memory\n");
        return 1;
    }

    for (int i = 0; i < noBytes; i++)
    {
        memory[i] = src[i];
    }
    printf("ROM file loaded into memory\n");
    return 0;
}

void memDump()
{

    // pointer to start of ROM memory
    unsigned char *ROM;

    // Point at beginning of memory
    ROM = memory;

    printf("Size of memory is %lu KBytes\n", sizeof(memory) / 1024);

    for (int i = 0; i < 5 /*sizeof(totalMem)*/; i++)
    {
        if (ROM[i] == 0x0d)
            ROM[i] = 'D';
        if (ROM[i] == 0x09)
            ROM[i] = '9';
        if (ROM[i] == 0x10)
            ROM[i] = '1';
        if (ROM[i] == 0x0a)
            ROM[i] = 'a';

        // std::bitset<8> b1(ch);
        printf("Byte %d\t%04X\t\n", i, memory[i]);
    }
}

uint8_t *getByte(uint16_t addrs) { return &memory[addrs]; }

uint16_t read_word(uint16_t addrs)
{
    // lo first [addrs +]
    // hi second [addrs]
    // bit shift hi to the left 8 places
    // OR 16-bit word with lo
    return ((uint16_t)memory[addrs + 1] | ((uint16_t)memory[addrs] << 8));
}
void write_word(uint16_t addr, uint16_t value) { memory[addr] = value; }
void write_byte(uint16_t addr, uint8_t val) { memory[addr] = val; }

uint16_t *getWord(uint16_t addrs)
{
    // Cast the address of the first byte referenced by addrs to a unint16_t to
    // get addrs+1
    return (uint16_t *)&memory[addrs];
}
