#ifndef INC_DISASSEMBLER_H
#define INC_DISASSEMBLER_H

#include <stdint.h>

#define DISASS_INSTRUCT_BUFFER_SIZE 30

typedef struct disass_instruction{
  const char addr[6];
  const char* instr;
}disass_instruction;

extern disass_instruction disass_instructions[DISASS_INSTRUCT_BUFFER_SIZE];

void initDisassInstructionsBuffer();
void populateInstructionsBuffer();
const char* disassemble_single_byte_opcode(uint8_t *opcode);
void disassemble_multi_byte_opcode(uint8_t *opcode );

#endif
