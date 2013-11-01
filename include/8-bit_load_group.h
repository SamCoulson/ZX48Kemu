#ifndef INC_8BITLOADGROUP_H
#define INC_8BITLOADGROUP_H 

#include <stdint.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LOAD data methods are used to load data from memory to register

// *** 8-BIT LOAD GROUP ***

// LD r,r'
// Copy 8-bit register value to 8-bit register
void LD8BitRegTo8BitReg( uint8_t *dstReg, uint8_t *srcReg );

// LD r,n
// Copy 8-bit constant value to 8-bit register
void LD8BitIntTo8BitReg( uint8_t &dst, uint8_t src );

// LD r,(HL)
// Copy value from address in HL to 8-bit register
void LDAddrsOfHLTo8BitReg( uint8_t &reg );

// LD r,(IX+d)
// Copy value from address in IX+d to 8-bit register
void LDAddrsOfIXdTo8BitReg( uint8_t &reg, uint8_t disp );

// LD r,(IY+d)
// Copy value from address in IY+d HL to 8-bit register
void LDAddrsOfIYdTo8BitReg( uint8_t &reg, uint8_t disp );

// LD (HL), r
// Copy value of 8-bit register to address in HL
void LD8BitRegToAddrsOfHL( uint8_t &reg );

// LD (IX+d),r
// Copy value from 8-bit register to address in IX+d
void LD8BitRegToAddrsOfIXd( uint8_t &reg, uint8_t disp );

// LD (IY+d),r
// Copy value from 8-bit register to address in IY+d 
void LD8BitRegToAddrsOfIYd( uint8_t &reg, uint8_t disp );

// LD (HL),n
// Copy integer value in 8-bit register into address at HL 
void LD8BitIntToAddrsOfHL( uint8_t val );

// LD (IX+d),n
// Copy integer value in 8-bit register to address in IX+d
void LD8BitIntToAddrsOfIXd( uint8_t disp, uint8_t val );

// LD (IY+d),n
// Copy integer value in 8-bit register to address in IY+d 
void LD8BitIntToAddrsOfIYd( uint8_t disp, uint8_t val );

// LD A,(BC)
// Copy value in address of BC into 8-bit register A
void LDAddrsOfBCToA();

// LD A,(DE)
// Copy value in address of BC into 8-bit register A
void LDAddrsOfDEToA();

// LD A,(nn)
// Copy value in 16-bit integer address to 8-bit register A **Write to PORT**
void LDAddrsOf16BitIntToA( uint8_t &reg, uint8_t srcConstHO, uint8_t srcConstLO );

// LD (BC),A
// Copy value in 8-bit register A to address in BC
void LDAToAddrsOfBC();

// LD (DE),A
// Copy value in 8-bit register A to address in DE
void LDAToAddrsOfDE();

// LD (nn),A
// Copy 16-bit integer value at 16-bit register address to 8-bit register 
void LDAToAddrsOf16BitInt( uint8_t dstIntHO, uint8_t dstIntLO );

// LD A,I 
// Copy value in 8-bit register I to 8-bit register A, and alter flags
void LDIToA();

// LD A,R
// Copy value in 8-bit register R to 8-bit register A, and alter flags
void LDRToA();

// LD I,A
// Copy value in 8-bit register I to 8-bit register A
void LDAToI(); 

// LD R,A
// Copy value in 8-bit register R to 8-bit register A
void LDAToR();

#endif // INC_8BITLOADGROUP_H 
