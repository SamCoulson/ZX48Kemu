#ifndef INC_16BITLOADGROUP_H
#define INC_16BITLOADGROUP_H

#include <stdint.h>

// *** 16-BIT LOAD GROUP ***
//

// LD dd,nn
// Copy 16-bit constant to a 16-bit reg
// OpCodes: 0x01, 0x11, 0x21, 0x31

// LD IX,nn
// Copy 16-bit integer into the IX register
// OpCodes: 0xFD21

// LD IY,nn
// Copy 16-bit integer into the IY register
// OpCodes: 0xFD21

// LD HL,(nn)
// Copy value from address 16-bit constant address value to 16-bit register **Write to PORT**
// OpCodes: 0x2A

// LD dd,(nn)
// Copy value at 16-bit integer address to 16-bit register
// OpCodes: 0xED4B 

// LD IX,(nn)
// Copy value into 16-bit HL IX register at the 16-bit integer address
// OpCodes: 0xED2A

// LD IY,(nn)
// Copy value into 16-bit HL IY register at the 16-bit integer address
// OpCodes: 0xFD2A

// LD(nn),HL
// Copy 16-bit HL register value into 16-bit integer address 
// OpCodes: 0x22

// LD(nn),dd
// Copy 16-bit register value to address in 16-bit constant 
// OpCodes: 0xED43, 0xED53

// LD(nn),IX
// Copy 16-bit IX register value into 16-bit integer address
// OpCodes: 0xDD22

// LD(nn),IY
// Copy 16-bit IY register value into 16-bit integer address
// OpCodes: 0xFD22

// LD SP,HL
// Copy 16-bit HL register value into SP 16-bit register
// OpCodes: 0xF9

// LD SP,IX
// Copy 16-bit IX register value into SP 16-bit register
// OpCodes: 0xDDF9

// LD SP,IY
// Copy 16-bit IY register value into SP 16-bit register
// OpCodes: 0xFDF9

void LD16( uint16_t* dst, uint16_t* src);

// PUSH qq
// Copy 16-bit register value onto the top of the stack
// OpCodes: 0xC5, 0xD5, 0xE5 0xF5

// PUSH IX
// Copy 16-bit IX register value onto the top of the stack
// OpCodes: 0xDDE5

// PUSH IY
// Copy 16-bit IY register value onto the top of the stack
// OpCodes: 0xFDE5
//
void PUSH( uint16_t* spAddrs, uint16_t* sp, uint16_t* srcVal );

// POP qq
// Copy 2-bytes from top of stack in a 16-bit register
// OpCodes: 0xC1, 0xD1, 0xE1 0xF1

// POP IX
// Copy 2-bytes from top of stack in a 16-bit IX register
// OpCodes: 0xDDE1

// POP IY
// Copy 2-bytes from top of stack in a 16-bit IY register
// OpCodes: 0xFDE1

void POP( uint16_t* spAddrs, uint16_t* sp, uint16_t* dstVal );

#endif // INC_16BITLOADGROUP_H 
