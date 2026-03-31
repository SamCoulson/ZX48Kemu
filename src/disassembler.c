#include "../include/disassembler.h"
#include "../include/cpu.h"
#include "../include/memory.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

disass_instruction disass_instructions[DISASS_INSTRUCT_BUFFER_SIZE];

void init_disass_instruction_buffer()
{
    printf("clearing dissasembly buffer\n");
    char *default_value = "????";

    for (int i = 0;
         i < sizeof(disass_instructions) / sizeof(disass_instruction); i++)
    {
        strcpy(disass_instructions[i].addr, "???");
        disass_instructions[i].instr = default_value;
    }
}

void populate_next_instructions_buffer()
{
    printf("populating instruction buffeer\n");
    // simply subtracting from the PC wont work because the number substrated
    // might end up in the middle of a multi-byte instruction.
    uint16_t next_instruct_addr = *z80->pc;

    for (int i = 0; i < DISASS_INSTRUCT_BUFFER_SIZE; i++)
    {
        uint8_t *opcode = &memory[next_instruct_addr];

        // set the memory address to display
        char addrStr[5];
        sprintf(addrStr, "%04X", next_instruct_addr);
        strcpy(disass_instructions[i].addr, addrStr);

        if (*opcode == 0xcb || *opcode == 0xdd || *opcode == 0xed ||
            *opcode == 0xfd)
        {
            uint16_t full_opcode = read_word_at(next_instruct_addr);

            printf("Dissembling multiple byte opcode %X at %X\n", full_opcode,
                   next_instruct_addr);

            uint8_t group = (full_opcode >> 8) & 0xff;
            uint8_t op = full_opcode & 0xff;

            z80_instruction instr = disassemble_multi_byte_opcode(&group, &op);

            disass_instructions[i].value = full_opcode;
            disass_instructions[i].instr = instr.name;

            next_instruct_addr = next_instruct_addr + instr.byte_length;
        }
        else if (*opcode >= 0x00 && *opcode <= 0xFF)
        {
            // printf("Dissembling single byte opcode %X\n", *opcode);
            z80_instruction instr = disassemble_single_byte_opcode(opcode);
            disass_instructions[i].instr = instr.name;
            disass_instructions[i].value = *opcode;

            next_instruct_addr = next_instruct_addr + instr.byte_length;
        }
    }
}

z80_instruction disassemble_single_byte_opcode(uint8_t *opcode)
{
    return single_byte_instruction_lookup[*opcode];
}

z80_instruction disassemble_multi_byte_opcode(uint8_t *group, uint8_t *opcode)
{
    switch (*group)
    {
    case 0xCB:
        return cb_multi_byte_instruction_lookup[*opcode];
        break;
    case 0xDD:
        break;
    case 0xED:
        return ed_multi_byte_instruction_lookup[*opcode];
        break;
    case 0xFD:
        return fd_multi_byte_instruction_lookup[*opcode];
        break;
    default:
        break;
    }
}
