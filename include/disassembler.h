#ifndef INC_DISASSEMBLER_H
#define INC_DISASSEMBLER_H

#include "cpu.h"
#include <stdint.h>

#define DISASS_INSTRUCT_BUFFER_SIZE 30

typedef struct disass_instruction{
  char addr[6];
  uint8_t value;
  const char* instr;
}disass_instruction;

extern disass_instruction disass_instructions[DISASS_INSTRUCT_BUFFER_SIZE];

void initDisassInstructionsBuffer();
void populateInstructionsBuffer();
z80_instruction disassemble_single_byte_opcode(uint8_t *opcode);
const char* disassemble_multi_byte_opcode(uint8_t *opcode );

#endif
