#ifndef INC_DISASSEMBLER_H
#define INC_DISASSEMBLER_H

#include <stdint.h>

#define DISASS_INSTRUCT_BUFFER_SIZE 30

extern char* disass_instructions[DISASS_INSTRUCT_BUFFER_SIZE];

void initDisassInstructionsBuffer();
const char* disassemble_single_byte_opcode(uint8_t *opcode);
void disassemble_multi_byte_opcode(uint8_t *opcode );

#endif
