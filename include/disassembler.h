
#ifndef INC_DISASSEMBLER_H
#define INC_DISASSEMBLER_H

#include <stdint.h>

void decode_single_byte_opcode(uint8_t *opcode );
void decode_multi_byte_opcode(uint8_t *opcode );

#endif
