#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "../include/disassembler.h"
#include "../include/cpu.h"
#include "../include/memory.h"

disass_instruction disass_instructions[DISASS_INSTRUCT_BUFFER_SIZE];

const char *multiByteInstructionLookup[];

void initDisassInstructionsBuffer()
{
  printf("clearing dissasembly buffer\n");
  char* default_value = "????";

  for( int i = 0; i < sizeof(disass_instructions) / sizeof(disass_instruction); i++)
  {
    strcpy(disass_instructions[i].addr, "???");
    disass_instructions[i].instr = default_value;
  }
}

void populateInstructionsBuffer()
{
    // simply subtracting from the PC wont work because the number substrated might end up in the middle of a multi-byte instruction.
    uint16_t next_instruct_addr = *z80->pc;

    for(int i = 0; i < DISASS_INSTRUCT_BUFFER_SIZE; i++)
    {
            uint8_t *opcode = &totalMem[next_instruct_addr];

            char addrStr[6];
            sprintf(addrStr, "%04X", next_instruct_addr);
            strcpy(disass_instructions[i].addr, addrStr);

            if( *opcode == 0xcb || *opcode == 0xdd || *opcode == 0xed || *opcode == 0xfd)
            {
                    disass_instructions[i].instr = disassemble_multi_byte_opcode( opcode );
                    next_instruct_addr++;
            }
            else if ( *opcode >= 0x00 && *opcode <= 0xff)
            {
                    z80_instruction instr = disassemble_single_byte_opcode( opcode );

                    disass_instructions[i].instr = instr.name;
                    disass_instructions[i].value = *opcode;
                    next_instruct_addr += instr.pc_skip_amount;
            }
    }
}

z80_instruction disassemble_single_byte_opcode(uint8_t *opcode)
{
  return singleByteInstructionLookup[*opcode];
}

const char* disassemble_multi_byte_opcode(uint8_t *opcode) 
{
  return multiByteInstructionLookup[(uint8_t)*opcode];
}

const char *multiByteInstructionLookup[] = {
    "RLC B",       /*0x00*/
    "RLC C",       /*0x01*/
    "RLC D",       /*0x02*/
    "RLC E",       /*0x03*/
    "RLC H",       /*0x04*/
    "RLC L",       /*0x05*/
    "RLC (HL)",    /*0x06*/
    "RLC A",       /*0x07*/
    "RRC B",       /*0x08*/
    "RRC C",       /*0x09*/
    "RRC D",       /*0x0A*/
    "RRC E",       /*0x0B*/
    "RRC H",       /*0x0C*/
    "RRC L",       /*0x0D*/
    "RRC (HL)",    /*0x0E*/
    "RRC A",       /*0x0F*/
    "RL B",        /*0x10*/
    "RL C",        /*0x11*/
    "RL D",        /*0x12*/
    "RL E",        /*0x13*/
    "RL H",        /*0x14*/
    "RL L",        /*0x15*/
    "RL (HL)",     /*0x16*/
    "RL A",        /*0x17*/
    "RR B",        /*0x18*/
    "RR C",        /*0x19*/
    "RR D",        /*0x1A*/
    "RR E",        /*0x1B*/
    "RR H",        /*0x1C*/
    "RR L",        /*0x1D*/
    "RR (HL)",     /*0x1E*/
    "RR A",        /*0x1F*/
    "SLA B",       /*0x20*/
    "SLA C",       /*0x21*/
    "SLA D",       /*0x22*/
    "SLA E",       /*0x23*/
    "SLA H",       /*0x24*/
    "SLA L",       /*0x25*/
    "SLA (HL)",    /*0x26*/
    "SLA A",       /*0x27*/
    "SRA B",       /*0x28*/
    "SRA B",       /*0x29*/
    "SRA D",       /*0x2A*/
    "SRA E",       /*0x2B*/
    "SRA H",       /*0x2C*/
    "SRA L",       /*0x2D*/
    "SRA (HL)",    /*0x2E*/
    "unused",      /*0x2F*/
    "unused",      /*0x30*/
    "unused",      /*0x31*/
    "unused",      /*0x32*/
    "unused",      /*0x33*/
    "unused",      /*0x34*/
    "unused",      /*0x35*/
    "unused",      /*0x36*/
    "unused",      /*0x37*/
    "SRL B",       /*0x38*/
    "SRL C",       /*0x39*/
    "SRL D",       /*0x3A*/
    "SRL E",       /*0x3B*/
    "SRL H",       /*0x3C*/
    "SRL L",       /*0x3D*/
    "SRL (HL)",    /*0x3E*/
    "SRL A",       /*0x3F*/
    "BIT 0",       /*0x40*/
    "BIT 0",       /*0x41*/
    "BIT 0",       /*0x42*/
    "BIT 0",       /*0x43*/
    "BIT 0",       /*0x44*/
    "BIT 0",       /*0x45*/
    "BIT 0",       /*0x46*/
    "BIT 0",       /*0x47*/
    "BIT 1",       /*0x48*/
    "BIT 1",       /*0x49*/
    "BIT 1",       /*0x4A*/
    "BIT 1",       /*0x4B*/
    "BIT 1",       /*0x4C*/
    "BIT 1",       /*0x4D*/
    "BIT 1",       /*0x4E*/
    "BIT 1",       /*0x4F*/
    "BIT 2",       /*0x50*/
    "BIT 2",       /*0x51*/
    "BIT 2",       /*0x52*/
    "BIT 2",       /*0x53*/
    "BIT 2",       /*0x54*/
    "BIT 2",       /*0x55*/
    "BIT 2",       /*0x56*/
    "BIT 2",       /*0x57*/
    "BIT 3",       /*0x58*/
    "BIT 3",       /*0x59*/
    "BIT 3,D",     /*0x5A*/
    "BIT 3",       /*0x5B*/
    "BIT 3",       /*0x5C*/
    "BIT 3",       /*0x5D*/
    "BIT 3",       /*0x5E*/
    "BIT 3",       /*0x5F*/
    "BIT 4",       /*0x60*/
    "BIT 4",       /*0x61*/
    "BIT 4",       /*0x62*/
    "BIT 4",       /*0x63*/
    "BIT 4",       /*0x64*/
    "BIT 4",       /*0x65*/
    "BIT 4",       /*0x66*/
    "BIT 4",       /*0x67*/
    "BIT 5",       /*0x68*/
    "BIT 5",       /*0x69*/
    "BIT 5",       /*0x6A*/
    "BIT 5",       /*0x6B*/
    "BIT 5",       /*0x6C*/
    "BIT 5",       /*0x6D*/
    "BIT 5, (HL)", /*0x6E*/
    "BIT 5",       /*0x6F*/
    "BIT 6, (HL)", /*0x70*/
    "BIT 6, (HL)", /*0x71*/
    "BIT 6, (HL)", /*0x72*/
    "BIT 6, (HL)", /*0x73*/
    "BIT 6, (HL)", /*0x74*/
    "BIT 6, (HL)", /*0x75*/
    "BIT 6, (HL)", /*0x76*/
    "BIT 6, (HL)", /*0x77*/
    "BIT 7, (HL)", /*0x78*/
    "BIT 7, (HL)", /*0x79*/
    "BIT 7, (HL)", /*0x7A*/
    "BIT 7, (HL)", /*0x7B*/
    "BIT 7, (HL)", /*0x7C*/
    "BIT 7, (HL)", /*0x7D*/
    "BIT 7, (HL)", /*0x7E*/
    "BIT 7, (HL)", /*0x7F*/
    "RES 0, (HL)", /*0x80*/
    "RES 0, (HL)", /*0x81*/
    "RES 0, (HL)", /*0x82*/
    "RES 0, (HL)", /*0x83*/
    "RES 0, (HL)", /*0x84*/
    "RES 0, (HL)", /*0x85*/
    "RES 0, (HL)", /*0x86*/
    "RES 0, (HL)", /*0x87*/
    "RES 1, (HL)", /*0x88*/
    "RES 1, (HL)", /*0x89*/
    "RES 1, (HL)", /*0x8A*/
    "RES 1, (HL)", /*0x8B*/
    "RES 1, (HL)", /*0x8C*/
    "RES 1, (HL)", /*0x8D*/
    "RES 1, (HL)", /*0x8E*/
    "RES 1, (HL)", /*0x8F*/
    "RES 2, (HL)", /*0x90*/
    "RES 2, (HL)", /*0x91*/
    "RES 2, (HL)", /*0x92*/
    "RES 2, (HL)", /*0x93*/
    "RES 2, (HL)", /*0x94*/
    "RES 2, (HL)", /*0x95*/
    "RES 2, (HL)", /*0x96*/
    "RES 2, (HL)", /*0x97*/
    "RES 3, (HL)", /*0x98*/
    "RES 3, (HL)", /*0x99*/
    "RES 3, (HL)", /*0x9A*/
    "RES 3, (HL)", /*0x9B*/
    "RES 3, (HL)", /*0x9C*/
    "RES 3, (HL)", /*0x9D*/
    "RES 3, (HL)", /*0x9E*/
    "RES 3, (HL)", /*0x9F*/
    "RES 4, (HL)", /*0xA0*/
    "RES 4, (HL)", /*0xA1*/
    "RES 4, (HL)", /*0xA2*/
    "RES 4, (HL)", /*0xA3*/
    "RES 4, (HL)", /*0xA4*/
    "RES 4, (HL)", /*0xA5*/
    "RES 4, (HL)", /*0xA6*/
    "RES 4, (HL)", /*0xA7*/
    "RES 5, (HL)", /*0xA8*/
    "RES 5, (HL)", /*0xA9*/
    "RES 5, (HL)", /*0xAA*/
    "RES 5, (HL)", /*0xAB*/
    "RES 5, (HL)", /*0xAC*/
    "RES 5, (HL)", /*0xAD*/
    "RES 5, (HL)", /*0xAE*/
    "RES 5, (HL)", /*0xAF*/
    "RES 6, (HL)", /*0xB0*/
    "RES 6, (HL)", /*0xB1*/
    "RES 6, (HL)", /*0xB2*/
    "RES 6, (HL)", /*0xB3*/
    "RES 6, (HL)", /*0xB4*/
    "RES 6, (HL)", /*0xB5*/
    "RES 6, (HL)", /*0xB6*/
    "RES 6, (HL)", /*0xB7*/
    "RES 7, (HL)", /*0xB8*/
    "RES 7, (HL)", /*0xB9*/
    "RES 7, (HL)", /*0xBA*/
    "RES 7, (HL)", /*0xBB*/
    "RES 7, (HL)", /*0xBC*/
    "RES 7, (HL)", /*0xBD*/
    "RES 7, H",    /*0xBE*/
    "RES 7, (HL)", /*0xBF*/
    "SET 0, (HL)", /*0xC0*/
    "SET 0, (HL)", /*0xC1*/
    "SET 0, (HL)", /*0xC2*/
    "SET 0, (HL)", /*0xC3*/
    "SET 0, (HL)", /*0xC4*/
    "SET 0, (HL)", /*0xC5*/
    "SET 0, (HL)", /*0xC6*/
    "SET 0, (HL)", /*0xC7*/
    "SET 1, (HL)", /*0xC8*/
    "SET 1, (HL)", /*0xC9*/
    "SET 1, (HL)", /*0xCA*/
    "SET 1, (HL)", /*0xCB*/
    "SET 1, (HL)", /*0xCC*/
    "SET 1, (HL)", /*0xCD*/
    "SET 1, (HL)", /*0xCE*/
    "SET 1, (HL)", /*0xCF*/
    "SET 2, (HL)", /*0xD0*/
    "SET 2, (HL)", /*0xD1*/
    "SET 2, (HL)", /*0xD2*/
    "SET 2, (HL)", /*0xD3*/
    "SET 2, (HL)", /*0xD4*/
    "SET 2, (HL)", /*0xD5*/
    "SET 2, (HL)", /*0xD6*/
    "SET 2, (HL)", /*0xD7*/
    "SET 3, (HL)", /*0xD8*/
    "SET 3, (HL)", /*0xD9*/
    "SET 3, (HL)", /*0xDA*/
    "SET 3, (HL)", /*0xDB*/
    "SET 3, (HL)", /*0xDC*/
    "SET 3, (HL)", /*0xDD*/
    "SET 3, (HL)", /*0xDE*/
    "SET 3, (HL)", /*0xDF*/
    "SET 4, (HL)", /*0xE0*/
    "SET 4, (HL)", /*0xE1*/
    "SET 4, (HL)", /*0xE2*/
    "SET 4, (HL)", /*0xE3*/
    "SET 4, (HL)", /*0xE4*/
    "SET 4, (HL)", /*0xE5*/
    "SET 4, (HL)", /*0xE6*/
    "SET 4, (HL)", /*0xE7*/
    "SET 5, (HL)", /*0xE8*/
    "SET 5, (HL)", /*0xE9*/
    "SET 5, (HL)", /*0xEA*/
    "SET 5, (HL)", /*0xEB*/
    "SET 5, (HL)", /*0xEC*/
    "SET 5, (HL)", /*0xED*/
    "SET 5, (HL)", /*0xEE*/
    "SET 5, (HL)", /*0xEF*/
    "SET 6, (HL)", /*0xF0*/
    "SET 6, (HL)", /*0xF1*/
    "SET 6, (HL)", /*0xF2*/
    "SET 6, (HL)", /*0xF3*/
    "SET 6, (HL)", /*0xF4*/
    "SET 6, (HL)", /*0xF5*/
    "SET 6, (HL)", /*0xF6*/
    "SET 6, (HL)", /*0xF7*/
    "SET 7, (HL)", /*0xF8*/
    "SET 7, (HL)", /*0xF9*/
    "SET 7, (HL)", /*0xFA*/
    "SET 7, (HL)", /*0xFB*/
    "SET 7, (HL)", /*0xFC*/
    "SET 7, (HL)", /*0xFD*/
    "SET 7, (HL)", /*0xFE*/
    "SET 7, L",    /*0xFF*/
};
