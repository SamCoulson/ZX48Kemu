#include "../include/16-bit_arithmetic_group.h"
#include "../include/cpu.h"
#include "../include/util_bit_operations.h"

#include <stdint.h>

// *** 16-bit Arithmetic group***

// ADD HL,ss
// Any of the register pairs BC, DE, HL or SP are added to the contents
// of the HL register
// OpCodes: 0x09, 0x19, 0x29, 0x39
uint8_t ADD_HL_DE(Z80 *z80)
{
    // ADD16(z80->hl, z80->de, z80->f);

    // H half carry, carry 1 from LO to HO
    (*z80->hl & 0x0FFF) + (*z80->de & 0x0FFF) > 0x0FFF ? setBit(z80->f, 4, 1)
                                                       : setBit(z80->f, 4, 0);
    // N is reset
    setBit(z80->f, 1, 0);

    // C is set to 1 if bit 15 carry out
    (*z80->hl + *z80->de) > 0xFFFF ? setBit(z80->f, 0, 1)
                                   : setBit(z80->f, 0, 0);

    // Add the two register values
    *z80->hl += *z80->de;

    ++*z80->pc;
    return 11;
}

// ADD IX,pp
// Any of the register pairs BC, DE, HL or SP are added to the contents
// of the IX register
// OpCodes: 0xDD09, 0xDD19, 0xDD29, 0xDD39

// ADD IY,rr
// Any of the register pairs BC, DE, HL or SP are added to the contents
// of the IY register
// OpCodes: 0xFD09, 0xFD19, 0xFD29, 0xFD39

void ADD16(uint16_t *dstReg, uint16_t *srcReg, uint8_t *fReg)
{
    printf("dstReg = %X", *dstReg);
    printf("srcReg = %X", *srcReg);
    // H half carry, carry 1 from LO to HO
    (*dstReg & 0x0FFF) + (*srcReg & 0x0FFF) > 0x0FFF ? setBit(fReg, 4, 1)
                                                     : setBit(fReg, 4, 0);
    // N is reset
    setBit(fReg, 1, 0);

    // C is set to 1 if bit 15 carry out
    (*dstReg + *srcReg) > 0xFFFF ? setBit(fReg, 0, 1) : setBit(fReg, 0, 0);

    // Add the two register values
    *dstReg += *srcReg;
}
/*
// ADC HL,ss
// Any of the register pairs BC, DE, HL or SP with the Carry flag
// are added to the contents of the HL register
// OpCodes: 0xED4A, 0xED5A, 0xED6A, 0x0xED7A
void ADC16BitRegToHL( uint8_t *HOreg, uint8_t *LOreg );
*/
// SBC HL,ss
// Any of the register pairs BC, DE, HL or SP with the Carry flag
// are subtracted to the contents of the HL register
// OpCodes: 0xED42, 0xED52, 0xED62, 0xED72
uint8_t SBC_HL_DE(Z80 *z80)
{
    SBC16(z80->hl, z80->de, z80->f);
    ++*z80->pc;
    return 15;
}

void SBC16(uint16_t *dstReg, uint16_t *srcReg, uint8_t *fReg)
{
    uint16_t sub = (*srcReg) + getBit(fReg, 0);

    // C is set if borrow occurs
    setBit(fReg, 0, (*dstReg < sub));

    // P/V overflow
    setBit(fReg, 2, (*dstReg < sub));

    // H if borrow at 12-bit boundary
    setBit(fReg, 4, (*dstReg & 0x0FFF) < (*srcReg & 0x0FFF) + getBit(fReg, 1));

    // N is set
    setBit(fReg, 1, 1);

    // Subtract dstreg and Carry flag from hlVal
    *dstReg -= sub;

    // S is 1 if result is signed
    (*dstReg & (1 << 15)) ? setBit(fReg, 7, 1) : setBit(fReg, 7, 0);

    // Z is set to 1 if result is 0
    setBit(fReg, 6, *dstReg == 0x00);
}

// INC ss
// Any of the register pairs BC, DE, HL or SP are incremented
// OpCodes: 0x03, 0x13, 0x23, 0x33
uint8_t INC_HL(Z80 *z80)
{
    INC16(z80->hl);
    ++*z80->pc;
    return 6;
}
// INC IX
// Contents of IX register is incremnted
// OpCodes: 0xDD23

// INC IY
// Contents of IX register is incremnted
// OpCodes: 0xFD23
void INC16(uint16_t *reg)
{
    // No flags are changed
    ++*reg;
}
// DEC ss
// Any of the register pairs BC, DE, HL or SP are decremented
// OpCodes: 0x0B, 0x1B, 0x2B, 0x3B
uint8_t DEC_HL(Z80 *z80)
{
    --*z80->hl;
    ++*z80->pc;
    return 6;
}
// DEC IX
// Contents of IX register is decremnted
// OpCodes: 0xDD2B

// DEC IY
// Contents of IX register is decremnted
// OpCodes: 0xFD2B

void DEC16(uint16_t *reg)
{
    // No flags are changed
    --*reg;
}
