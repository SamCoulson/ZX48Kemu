#include <stdio.h>
#include "../include/disassembler.h"


const char *singleByteInstructionLookup[];
const char *multiByteInstructionLookup[];

void disassemble_single_byte_opcode(uint8_t *opcode) {
  printf(" %s\n", singleByteInstructionLookup[(uint8_t)*opcode]);
}

void disassemble_multi_byte_opcode(uint8_t *opcode) {
  printf(" %s\n", multiByteInstructionLookup[(uint8_t)*opcode]);
}


const char *singleByteInstructionLookup[] = {
    "NOP (Not implemented yet!",
    "LD BC,+%X",
    "LD(BC),A",
    "INC BC",
    "INC B",
    "DEC B",
    "LD B,+%X",
    "RLCA",
    "EX AF,AF'",
    "ADD HL,BC",
    "LD A,(BC)",
    "DEC BC",
    "INC C",
    "DEC C",
    "LD C,+%X",
    "RRCA",
    "DJNZ %d",
    "LD DE,+%X",
    "LD(DE),A",
    "INC DE",
    "INC D",
    "DEC D",
    "LD D,+%X",
    "RLA",
    "JR %X",
    "ADD HL,DE",
    "LD A,(DE)",
    "DEC DE",
    "INC E",
    "DEC E",
    "LD E,+%X",
    "RRA",
    "JR NZ,%d",
    "LD HL,+%X",
    "LD (%X),HL",
    "INC HL",
    "INC H",
    "DEC H",
    "LD H,+%X",
    "DAA",
    "JR Z,+%X",
    "ADD HL,HL",
    "LD HL,(%X)",
    "DEC HL",
    "INC L",
    "DEC L",
    "LD L,+%X",
    "CPL",
    "JR NC,%d",
    "LD SP,+%X",
    "LD (%X),A",
    "INC SP",
    "INC (HL)",
    "DEC (HL)",
    "LD (HL),+%X",
    "SCF",
    "JR C,%d",
    "ADD HL,SP",
    "LD A,(%X)",
    "DEC SP",
    "INC A",
    "DEC A",
    "LD A,+%X",
    "CCF",
    "LD B,B",
    "LD B,C",
    "LD B,D",
    "LD B,E",
    "LD B,H",
    "LD B,L",
    "LD B,(HL)",
    "LD B,A",
    "LD C,B",
    "LD C,C",
    "LD C,D",
    "LD C,E",
    "LD C,H",
    "LD C,L",
    "LD C,(HL)",
    "LD C,A",
    "LD D,B",
    "LD D,C",
    "LD D,D",
    "LD D,E",
    "LD D,H",
    "LD D,L",
    "LD D,(HL)",
    "LD D,A",
    "LD E,B",
    "LD E,C",
    "LD E,D",
    "LD E,E",
    "LD E,H",
    "LD E,L",
    "LD E,(HL)",
    "LD E,A",
    "LD H,B",
    "LD H,C",
    "LD H,D",
    "LD H,E",
    "LD H,H",
    "LD H,L",
    "LD H,(HL)",
    "LD H,A",
    "LD L,B",
    "LD L,C",
    "LD L,D",
    "LD L,E",
    "LD L,H",
    "LD L,L",
    "LD L,(HL)",
    "LD L,A",
    "LD (HL),B",
    "LD (HL),C",
    "LD (HL),D",
    "LD (HL),E",
    "LD (HL),H",
    "LD (HL),L",
    "HALT",
    "LD (HL),A",
    "LD A,B",
    "LD A,C",
    "LD A,D",
    "LD A,E",
    "LD A,H",
    "LD A,L",
    "LD A,(HL)",
    "LD A,A",
    "ADD A,B",
    "ADD A,C",
    "ADD A,D",
    "ADD A,E",
    "ADD A,H",
    "ADD A,L",
    "ADD (HL)",
    "ADD A,A",
    "ADC A,B",
    "ADC A,C",
    "ADC A,D",
    "ADC A,E",
    "ADC A,H",
    "ADC A,L",
    "ADC A,(HL)",
    "ADC A,A",
    "SUB B",
    "SUB C",
    "SUB D",
    "SUB E",
    "SUB H",
    "SUB L",
    "SUB (HL)",
    "SUB A",
    "SBC A,B",
    "SBC A,C",
    "SBC A,D",
    "SBC A,E",
    "SBC A,H",
    "SBC A,L",
    "SBC A,(HL)",
    "SBC A,A",
    "AND B",
    "AND C",
    "AND D",
    "AND E",
    "AND H",
    "AND L",
    "AND (HL)",
    "AND A",
    "XOR B",
    "XOR C",
    "XOR D",
    "XOR E",
    "XOR H",
    "XOR L",
    "XOR (HL)",
    "XOR A",
    "OR B",
    "OR C",
    "OR D",
    "OR E",
    "OR H",
    "OR L",
    "OR(HL)",
    "OR A",
    "CP B",
    "CP C",
    "CP D",
    "CP E",
    "CP H",
    "CP L",
    "CP (HL)",
    "CP A",
    "RET NZ",
    "POP BC",
    "JP NZ, %X",
    "JP %X",
    "CALLNZ %X",
    "PUSH BC",
    "ADD A,%X",
    "RST 00H",
    "RET Z",
    "RET",
    "JP Z,%X",
    "CALL Z,%X",
    "CALL %X",
    "ADC A,%X",
    "RST 08H",
    "RET NC",
    "POP DE",
    "JP NC,%X",
    "OUT (+%X),A",
    "CALL NC,%X",
    "PUSH DE",
    "SUB %X",
    "RST 10",
    "RET C",
    "EXX",
    "JP C,%X",
    "IN A,(C)",
    "CALL C,%X",
    "SBC A,%X",
    "RST 18H",
    "RET PO,%X",
    "POP HL",
    "JP PO,%X ",
    "EX (SP),HL",
    "CALL PO,%X",
    "PUSH HL",
    "AND %X",
    "RST 20H",
    "RET PE,%X",
    "JP (HL)",
    "JP PE,%X",
    "EX DE,HL",
    "CALL PE,%X",
    "XOR %X",
    "RST 28H",
    "RET P",
    "POP AF",
    "JP P,%X",
    "DI",
    "CALL P,%X",
    "PUSH AF",
    "OR %X",
    "RST 30H",
    "RET M,%X",
    "LD SP,HL",
    "JP M,%X",
    "EI",
    "CALL M,%X",
};

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
