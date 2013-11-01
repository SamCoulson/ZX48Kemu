#ifndef INC_16BITLOADGROUP_H
#define INC_16BITLOADGROUP_H

// *** 16-BIT LOAD GROUP ***
//

// LD dd,nn
// Copy 16-bit constant to a 16-bit reg
// OpCodes: 0x01, 0x11, 0x21, 0x31
void LD16BitConstTo16BitReg( uint8_t *dstHO, uint8_t *dstLO, uint8_t srcHO, uint8_t srcLO );

// LD IX,nn
// Copy 16-bit integer into the IX register
// OpCodes: 0xFD21
void LD16BitIntToIXReg( uint8_t srcHO, uint8_t srcLO );

// LD IY,nn
// Copy 16-bit integer into the IY register
// OpCodes: 0xFD21
void LD16BitIntToIYReg( uint8_t srcHO, uint8_t srcLO );

// LD HL,(nn)
// Copy value from address 16-bit constant address value to 16-bit register **Write to PORT**
// OpCodes: 0x2A
void LDAddrsOf16BitIntToHLReg(  uint8_t srcIntHO, uint8_t srcIntLO );

// LD dd,(nn)
// Copy value at 16-bit integer address to 16-bit register
// OpCodes: 0xED4B 
void LDAddrsOf16BitIntTo16BitReg( uint8_t *regHO, uint8_t *regLO, uint8_t srcIntHO, uint8_t srcIntLO );

// LD IX,(nn)
// Copy value into 16-bit HL IX register at the 16-bit integer address
// OpCodes:
void LD16BitAddrsOf16BitIntToIX( uint8_t srcIntHO, uint8_t srcIntLO );

// LD IY,(nn)
// Copy value into 16-bit HL IY register at the 16-bit integer address
// OpCodes: 0xFD2A
void LD16BitAddrsOf16BitIntToIY( uint8_t srcIntHO, uint8_t srcIntLO );

// LD(nn),HL
// Copy 16-bit HL register value into 16-bit integer address 
// OpCodes: 0x22
void LDHLRegToAddrsOf16BitInt( uint8_t dstIntHO, uint8_t dstIntLO );

// LD(nn),dd
// Copy 16-bit register value to address in 16-bit constant 
// OpCodes: 0xED43, 0xED53
void LD16BitRegToAddrsOf16BitInt( uint8_t dstIntHO, uint8_t dstIntLO, uint8_t *srcRegHO, uint8_t *srcRegLO );

// LD(nn),IX
// Copy 16-bit IX register value into 16-bit integer address
// OpCodes: 0xDD22
void LDIXRegToAddrsOf16BitInt( uint8_t dstIntHO, uint8_t dstIntLO );

// LD(nn),IY
// Copy 16-bit IY register value into 16-bit integer address
// OpCodes: 0xFD22
void LDIYRegToAddrsOf16BitInt( uint8_t dstIntHO, uint8_t dstIntLO );

// LD SP,HL
// Copy 16-bit HL register value into SP 16-bit register
// OpCodes: 0xF9
void LDHLRegToSPReg();

// LD SP,IX
// Copy 16-bit IX register value into SP 16-bit register
// OpCodes: 0xDDF9
void LDIXRegToSPReg();

// LD SP,IY
// Copy 16-bit IY register value into SP 16-bit register
// OpCodes: 0xFDF9
void LDIYRegToSPReg(); 

// PUSH qq
// Copy 16-bit register value onto the top of the stack
// OpCodes: 0xC5, 0xD5, 0xE5 0xF5
void PUSH( uint8_t *regHO, uint8_t *regLO );

// PUSH IX
// Copy 16-bit IX register value onto the top of the stack
// OpCodes: 0xDDE5
void PUSHIX();

// PUSH IY
// Copy 16-bit IY register value onto the top of the stack
// OpCodes: 0xFDE5
void PUSHIY();

// POP qq
// Copy 2-bytes from top of stack in a 16-bit register
// OpCodes: 0xC1, 0xD1, 0xE1 0xF1
void POP( uint8_t *regHO, uint8_t *regLO );

// POP IX
// Copy 2-bytes from top of stack in a 16-bit IX register
// OpCodes: 0xDDE1
void POPIX();

// POP IY
// Copy 2-bytes from top of stack in a 16-bit IY register
// OpCodes: 0xFDE1
void POPIY();

#endif // INC_16BITLOADGROUP_H 
