#ifndef INC_IOGROUP_H
#define INC_IOGROUP_H 

#include <stdint.h>

//////////////////////////////////////////////////////////////////////////

// *** Input and Output group ***

// IN A,(n)
// Read in one byte from IO mapped port (n) into register A 
// OpCodes: 0xDB
void INA( uint8_t *aReg, uint8_t port );

// IN r,(C)
// IO ported is slected through contents of register C and one 
// byte copied in to register r
// OpCodes: 0xED40, 0xED48, 0xED50, 0xED58, 0xED60, 0xED68, 0xED78 
void INAddrsOfRegCTo8BitReg( uint8_t *reg );

// INI
// IO port is selected by the contents of register C, the byte is 
// read in and placed into the contents of the HL register, the byte
// counter is decremented and the HL incremented
// OpCodes: 0xEDA2
void INI();

// INIR
// IO port is selected by the contents of register C, the byte is 
// read in and placed into the contents of the HL register, if B is non-zero
// repeat instruction
// OpCodes: 0xEDB2
void INIR();

// IND
// IO port is selected by the contents of register C, the byte is 
// read in and placed into the contents of the HL register, the byte
// counter is incremented and the HL incremented
// OpCodes: 0xEDAA
void IND();

// INDR
// IO port is selected by the contents of register C, the byte is 
// read in and placed into the contents of the HL register, if B is non-zero
// repeat instruction
// OpCodes: 0xEDBA
void INDR();	

// OUT (n),A
// The output port is selected with address (almost always FE) of the integer along with 
// register A, the byte in register A is copied to the address where the bits in the byte 
// are read the action taken depending on which bits are on/off
// OpCodes: 0xD3
void OUTA( uint8_t* port, uint8_t *val ); 

// OUT (C),r
// Contents of register C is used to select an ouput port, byte in register
// r is copied to the port
// OpCodes: 0xED41, 0xED49, 0xED51, 0xED59, 0xED61, 0xED69, 0xED79
void OUTC( uint8_t *reg );

// OUTI
// Output to port referecne by contents of address in HL register
// register B is used as a counter and is decremented
// OpCodes: 0xEDA3
void OUTI();

// OTIR
// Output to port referecne by contents of address in HL register
// register B is used as a counter and is decremented, the instruction is 
// repeated if B is non-zero
// OpCodes: 0xEDB3
void OTIR();	

// OUTD
// Output to port referecne by contents of address in HL register
// register B is used as a counter and is decremented
// OpCodes: 0xEDAB
void OUTD();	

// OTDR
// Output to port referecne by contents of address in HL register
// register B is used as a counter and is decremented, the instruction is 
// repeated if B is non-zero
// OpCodes: 0xEDBB
void OTDR();	

#endif // INC_IOGROUP_H
