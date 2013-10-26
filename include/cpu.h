#ifndef INC_CPU_H
#define INC_CPU_H

#include <iostream>
#include <stdint.h>
#include "memory.h"

class CPU{
	public:
		CPU(Memory* memPtr);

		// Strat the CPU running
		void start();

		// Set program counter to a pointer to a memory address
		void setPC(uint16_t mem);

	private:

		Memory* memPtr; 

		// 8-Bit Z80 main general purpose registers
		uint8_t regA;		// Accumulater
		uint8_t regF;		// Flags register (Set = 1, Reset = 0)
		uint8_t regB;		// Byte counter
		uint8_t regC;
		uint8_t regD;
		uint8_t regE;
		uint8_t regH;
		uint8_t regL;

		// 8-Bit Z80 alternative general purpose registers
		uint8_t aregA;
		uint8_t aregF; 
		uint8_t aregB;
		uint8_t aregC;
		uint8_t aregD;
		uint8_t aregE;
		uint8_t aregH;
		uint8_t aregL;

		// 8-Bit special registers - Interrupt vector
		uint8_t regI;
		uint8_t regR;

		// 16-Bit index registers
		uint16_t indexIX;
		uint16_t indexIY;

		// 16-Bit stack pointer and program counter 
		uint16_t sp;
		uint16_t pc; // AKA Instruction pointer

		// IFFI (Interrupt enabled flip-flop)
		bool IFF1;
		bool IFF2;

		// Execute instructions
		void execute(uint8_t* opcode);

		// ARITHMETIC OPERATIONS

		// ADD first 16-bit register to the second store in the first
		void ADD(uint8_t &reg1, uint8_t &reg2, uint8_t &reg3, uint8_t &reg4);

		// SBC subtract w/ carry flag the contents of the second register and the c flag from the first registers
		void SBC(uint8_t &reg1, uint8_t &reg2, uint8_t &reg3, uint8_t &reg4);

		// INC 16-bit
		void INC(uint8_t &reg1, uint8_t &reg2);

		// INC 8-bit
		void INC(uint8_t &reg);

		// DEC value at the address of 16-bit register
		void DEC(uint8_t &HOreg, uint8_t &LOreg);

		// LOGICAL OPERATIONS

		// XOR value with itself
		void XOR( uint8_t &val );

		void AND(uint8_t &reg);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// LOAD data methods are used to load data from memory to register

		// *** 8-BIT LOAD GROUP ***

		// LD r,r'
		// Copy 8-bit register value to 8-bit register
		void LD8BitRegTo8BitReg( uint8_t &dstReg, uint8_t &srcReg );

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

		// *** 16-BIT LOAD GROUP ***
		//

		// LD dd,nn
		// Copy 16-bit constant to a 16-bit reg
		// OpCodes: 0x01, 0x11, 0x21, 0x31
		void LD16BitConstTo16BitReg( uint8_t &dstHO, uint8_t &dstLO, uint8_t srcHO, uint8_t srcLO );
		
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
		void LDAddrsOf16BitIntTo16BitReg( uint8_t &regHO, uint8_t &regLO, uint8_t srcIntHO, uint8_t srcIntLO );
		
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
		void LD16BitRegToAddrsOf16BitInt( uint8_t dstIntHO, uint8_t dstIntLO, uint8_t &srcRegHO, uint8_t &srcRegLO );

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
		void PUSH( uint8_t &regHO, uint8_t &regLO );

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
		void POP( uint8_t &regHO, uint8_t &regLO );

		// POP IX
		// Copy 2-bytes from top of stack in a 16-bit IX register
		// OpCodes: 0xDDE1
		void POPIX();

		// POP IY
		// Copy 2-bytes from top of stack in a 16-bit IY register
		// OpCodes: 0xFDE1
		void POPIY();	
						
		// ***EXCHANGE, BLOCK TRANSFER AND SEARCH GROUPS***

		// EX DE,HL
		// Swap DE and HL register values
		// OpCodes: 0xEB
		void EXDEHL();
		
		// EX AF,AF'
		// Swap register values between AF and alternative AF 
		// OpCodes: 0x08
		void EXAFAltAF();
		
		// EXX
		// Swap register BC & BC', DE & DE', and HL & HL'
		// OpCodes: 0xD9
		void EXX();

		// EX (SP),HL
		// Swap values between two HL register and 2-bytes on stack at SP
		// OpCodes: 0xE3
		void EXHLToAddrsOfSP();
		
		// EX (SP),IX
		// Swap values between two HL register and 2-bytes on stack at SP
 		// OpCodes: 0xDDE3
		void EXIXToAddrsOfSP();

		// EX (SP),IY
		// Swap values between two IX register and 2-bytes on stack at SP 
		// OpCodes: 0xFDE3
		void EXIYToAddrsOfSP();

		// LDI *Changes flags*
		// Copy value at adress in HL register to address in DE register
		// Increment both HL and BC, and decrement BC
		// OpCodes: 0xEDA0
		void LDI();

		// LDIR *Changes flags*
		// Copy value at adress in HL register to address in DE register
		// Increment both HL and BC, and decrement BC
		// OpCodes: 0xEDB0
		void LDIR();
	
		// LDD *Changes flags* 
		// Copy value at adress in HL register to address in DE register
		// Decrement both HL and BC, and decrement BC
		// OpCodes: 0xEDA8
		void LDD();

		// LDDR *Changes flags*
		// Copy value at adress in HL register to address in DE register
		// Decrement both HL and BC, and decrement BC
		// OpCodes: 0xEDB8
		void LDDR();

		// CPI *Changes flags*
		// Contents of HL compared with contents of A
		// OpCodes: 0xEDA1
		void CPI();

		// CPIR *Change flags*
		// Contents of HL compared with contents of A
		// HL and BC affetced by outcome of comparison
		// OpCodes: 0xEDB1
		void CPIR();

		// CPD
		// Contents of HL compared with contents of A
		// HL and BC affetced by outcome of comparison 
		// OpCodes: 0xEDA9
		void CPD();

		// CPDR
		// Contents of HL compared with contents of A
		// HL and BC affetced by outcome of comparison 
		// OpCodes: 0xEDB9
		void CPDR();

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// *** 8-Bit Arithmetic group*** 

		// ADD A,s
		// Adds an a 8-bit value to the accumulater
		// ADD A,r 
		// OpCodes: 0x80, 0x81, 0x82, 0x83 ,0x84 ,0x85 ,0x87
		// ADD A,n
		// Add an 8-bit integer to register A
		// OpCodes: 0xC6
		// ADD A,(HL)
		// Add the value at the address of HL to register A
		// OpCodes: 0x86
		// ADD A,(IX+d)
		// Add the value at address of IX plus offset to register A
		// OpCodes: 0xDD86 
		// ADD A,(IY+d)
		// Add the value at address of IX plus offset to register A
		// OpCodes: 0xFD86 	
		void ADD8BitValToA( uint8_t val );

		// ADC A,r
		// Add an 8-bit integer + C Flag to register A  
		// OpCodes: 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8F
		// ADC A,n
		// Add an 8-bit integer + C Flag to register A
		// OpCodes: 0xCE
		// ADC A,(HL)
		// Add the value at the address of HL + C Flag to register A
		// OpCodes: 0x8E
		// ADC A,(IX+d)
		// Add the value at address of IX plus offset + C Flag to register A
		// OpCodes: 0xDD8E
		// ADC A,(IY+d)
		// Add the value at address of IX plus offset + C Flag to register A
		// OpCodes: 0xFD8E
		void ADC8BitValToA( uint8_t val );
	
		// SUB A,s
		// Subtract an a 8-bit value from the accumulater
		// SUB A,r 
		// OpCodes: 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x97 
		// SUB A,n
		// Subtract an 8-bit integer from register A
		// OpCodes: 0xD6
		// SUB A,(HL)
		// Subtract the value at the address of HL from register A
		// OpCodes: 0x96
		// SUB A,(IX+d)
		// Subtract the value at address of IX plus offset from register A
		// OpCodes: 0xDD96
		// SUB A,(IY+d)
		// Subtract the value at address of IX plus offset from register A
		// OpCodes: 0xFD96
		void SUB8BitValFromA( uint8_t val ); 	
					
		// SBC A,r
		// Subtract an 8-bit integer - C Flag from register A  
		// OpCodes: 0x9F, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D 
		// SBC A,n
		// Subtract an 8-bit integer - C Flag from register A
		// OpCodes: 0xDE
		// SBC A,(HL)
		// Subtract the value at the address of HL - C Flag from register A
		// OpCodes: 0x9E
		// SBC A,(IX-d)
		// Subtract the value at address of IX plus offset - C Flag from register A
		// OpCodes: 0xDD9E
		// ADD A,(IY-d)
		// Subtract the value at address of IX plus offset - C Flag from register A
		// OpCodes: 0xFD9E
		void SBC8BitValFromA( uint8_t val );
	
		// AND A,s
		// Logical AND an a 8-bit value with the accumulater
		// AND A,r 
		// OpCodes: 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA7  
		// AND A,n
		// Logical AND an 8-bit integer with register A
		// OpCodes: 0xE6
		// AND A,(HL)
		// Logical AND the value at the address of HL with register A
		// OpCodes: 0xA6
		// AND A,(IX+d)
		// Logical AND the value at address of IX plus offset with register A
		// OpCodes: 0xDDA6 
		// AND A,(IY+d)
		// Logical AND the value at address of IX plus offset with register A
		// OpCodes: 0xFDA6
		void AND8BitValWithA( uint8_t val ); 	

		// OR,s		
		// OR A,s
		// Logical OR an a 8-bit value with the accumulater
		// OR A,r 
		// OpCodes: 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB7  
		// OR A,n
		// Logical OR an 8-bit integer with register A
		// OpCodes: 0xF6
		// OR A,(HL)
		// Logical OR the value at the address of HL with register A
		// OpCodes: 0xB6
		// OR A,(IX+d)
		// Logical OR the value at address of IX plus offset with register A
		// OpCodes: 0xDDB6
		// OR A,(IY+d)
		// Logical OR the value at address of IX plus offset with register A
		// OpCodes: 0xFDB6
		void OR8BitValWithA( uint8_t val ); 	
		
		// XOR,s		
		// Logical XOR an a 8-bit value with the accumulater
		// XOR A,r 
		// OpCodes: 0xA8, 0xA9,0xAA, 0xAB, 0xAC, 0xAD, 0xAF   
		// XOR A,n
		// Logical XOR an 8-bit integer with register A
		// OpCodes: 0xEE
		// XOR A,(HL)
		// Logical XOR the value at the address of HL with register A
		// OpCodes: 0xAE
		// XOR A,(IX+d)
		// Logical XOR the value at address of IX plus offset with register A
		// OpCodes: 0xDDAE
		// XOR A,(IY+d)
		// Logical XOR the value at address of IX plus offset with register A
		// OpCodes: 0xFDAE
		void XOR8BitValWithA( uint8_t val ); 	
		
		// CP,s		
		// Logical CP an a 8-bit value with the accumulater
		// CP A,r 
		// OpCodes: 0xB8, 0xB9,0xBA, 0xBB, 0xBC, 0xBD, 0xBF    
		// CP A,n
		// Logical CP an 8-bit integer with register A
		// OpCodes: 0xFE
		// CP A,(HL)
		// Logical CP the value at the address of HL with register A
		// OpCodes: 0xBE
		// CP A,(IX+d)
		// Logical CP the value at address of IX plus offset with register A
		// OpCodes: 0xDDBE 
		// CP A,(IY+d)
		// Logical CP the value at address of IX plus offset with register A
		// OpCodes: 0xFDBE
		void CP8BitValWithA( uint8_t val ); 	
		
		// INC r
		// Increment an 8-Bit register
		// OpCodes: 0x3C, 0x04, 0x0C, 0x14, 0x1C, 0x24, 0x2C,  
		void INC8BitReg( uint8_t &reg );

		// INC(HL)
		// Increment the contents on address pointed to by HL register
		// OpCodes: 0x34
		void INCAddrsOfHL( uint8_t &reg );
	
		// INC(IX+d)
		// Increment the contents of address plus offset in IX register
		// OpCodes: 0xDD34
		void INCAddrsOfIXOffset( uint8_t &reg );

		// INC(IY+d)
		// Increment the contents of address plus offset in IX register
		// OpCodes: 0xFD34
		void INCAddrsOfIYOffset( uint8_t &reg );

		// DEC r
		// Decrement an 8-Bit register
		// OpCodes: 0x3C, 0x04, 0x0C, 0x14, 0x1C, 0x24, 0x2C,  
		void DEC8BitReg( uint8_t &reg );

		// DEC(HL)
		// Decrement the contents on address pointed to by HL register
		// OpCodes: 0x34
		void DECAddrsOfHL( uint8_t &reg );
	
		// DEC(IX+d)
		// Decrement the contents of address plus offset in IX register
		// OpCodes: 0xDD34
		void DECAddrsOfIXOffset( uint8_t &reg );

		// DEC(IY+d)
		// Decrement the contents of address plus offset in IX register
		// OpCodes: 0xFD34
		void DECAddrsOfIYOffset( uint8_t &reg );
		
		// CONTROL FLOW

		// Jump to the memory location using a 16-bit address specified by the combined 8-bit values  
		void JP( uint8_t *byte1, uint8_t *byte2 );

		// Jump relative when z not 0 - Jump by val if Z in flags is non-zero
		void JRNZ( uint8_t *val );

		// Jump relative when c not 0 - Jump by val if c flag is non-zero *check this*
		void JRNC( uint8_t *val );

		// Jump relative when z flag is 0
		void JRZ( uint8_t *val );

		void OUTA( uint8_t ioAddrs, uint8_t* val);

		// Compare the value in a single8-bit register
		void CP(uint8_t &reg);

		// Utility functions

		// Memory handling

		// Read byte from memory given 16-bit address
		uint8_t readByte( uint16_t romAddrs );

		// Write byte to memory given 16-bit address
		void writeByte( uint16_t addrs, uint8_t val );

		// Set the given bit in a byte to either 1 or 0
		void setBitInByte(uint8_t &byte, uint8_t pos, uint8_t val);

		bool IsBitSet( uint8_t &byte, uint8_t pos );

		// But first byte in to HO byte, and second byte in to LO byte of a word
		uint16_t byteToWord(uint8_t *byte1, uint8_t *byte2);

		// Return the LO from a word
		uint8_t getLOByte(uint16_t *word);

		// Return the Ho from a word
		uint8_t getHOByte(uint16_t *word);
		
		////////////////////////////////////////////////////////////////////////////////////////////
		
		// ***General-Purpose Arithmetic and CPU Control Group***
		
		// DAA
		// Conditionally adjust the accumulator for BCD conditions
		// OpCodes: 0x27

		// CPL
		// Contents of accumulator A are inverted to the complement
		// OpCodes: 0x2F 
		
		// NEG
		// The contentsof the accumulator is negated (subtracted from 0)
		// OpCodes: 0xED44
		
		// CCF
		// The carry flag in the F register is negated
		// OpCodes: 0x3F

		// SCF
		// The carry flag in the F register is set
		// OpCodes: 0x37

		// NOP
		// Perform no operation during the machine cycle
		// OpCodes: 0x00

		// HALT
		// Suspend CPU operation until an interupt is received
		// OpCodes: 0x76

		// DI
		// Diable the maskable interupts IFF1 and IFF2 by resetting enable interupt flip-flop
		// OpCodes: 0xF3
		void DI();

		// EI
		// Enable interupt instruction sets, sets both IFF1 and IFF2.
		// OpCodes: 0xFB
		 
		// IM 0
		// Set interrupt mode 0
		// OpCodes: 0xED46

		// IM 1
		// Set interrupt mode 1
		// opCodes: 0xED56
 
		// IM 2
		// Set interrupt mode 2
		// OpCodes: 0xED5E


};

#endif // INC_CPU_H
