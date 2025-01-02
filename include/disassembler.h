#ifndef INC_DISASSEMBLER_H
#define INC_DISASSEMBLER_H

#include <stdint.h>

void disassemble_single_byte_opcode(uint8_t *opcode );
void disassemble_multi_byte_opcode(uint8_t *opcode );

#endif
