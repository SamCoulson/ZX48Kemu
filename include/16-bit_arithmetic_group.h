#ifndef INC_16BITARITHMETICGROUP_H
#define INC_16BITARITHMETICGROUP_H

#include <stdint.h>

/////////////////////////////////////////////////////////////////////////////////////////////
//	
// *** 16-bit Arithmetic group***
//

// ADD HL,ss
// Any of the register pairs BC, DE, HL or SP are added to the contents
// of the HL register
// OpCodes: 0x09, 0x19, 0x29, 0x39
void ADD16( uint16_t* dstReg, uint16_t* srcReg, uint8_t* fReg );
/*
// ADC HL,ss
// Any of the register pairs BC, DE, HL or SP with the Carry flag 
// are added to the contents of the HL register
// OpCodes: 0xED4A, 0xED5A, 0xED6A, 0x0xED7A
void ADC16BitRegToHL( uint8_t &HOreg, uint8_t &LOreg );	
*/
// SBC HL,ss
// Any of the register pairs BC, DE, HL or SP with the Carry flag 
// are subtracted to the contents of the HL register
// OpCodes: 0xED42, 0xED52, 0xED62, 0xED72
void SBC16( uint16_t *dstReg, uint16_t *srcReg, uint8_t* fReg );

/*
// ADD IX,pp
// Any of the register pairs BC, DE, HL or SP are added to the contents
// of the IX register
// OpCodes: 0xDD09, 0xDD19, 0xDD29, 0xDD39
void ADD16BitRegToIX( uint8_t &HOreg, uint8_t &LOreg );

// ADD IY,rr
// Any of the register pairs BC, DE, HL or SP are added to the contents
// of the IY register
// OpCodes: 0xFD09, 0xFD19, 0xFD29, 0xFD39
void ADD16BitRegToIY( uint8_t &HOreg, uint8_t &LOreg );
*/
// INC ss
// Any of the register pairs BC, DE, HL or SP are incremented
// OpCodes: 0x03, 0x13, 0x23, 0x33
//void INC16BitReg( uint8_t &HOreg, uint8_t &LOreg );		

// INC IX
// Contents of IX register is incremnted
// OpCodes: 0xDD23
//void INCIX();	

// INC IY
// Contents of IX register is incremnted
// OpCodes: 0xFD23
//void INCIY();	
void INC16( uint16_t* );

// DEC ss
// Any of the register pairs BC, DE, HL or SP are decremented
// OpCodes: 0x0B, 0x1B, 0x2B, 0x3B
//void DEC16BitReg( uint8_t &HOreg, uint8_t &LOreg );		

// DEC IX
// Contents of IX register is decremnted
// OpCodes: 0xDD2B
//void DEXIX();

// DEC IY
// Contents of IX register is decremnted
// OpCodes: 0xFD2B
//void DECIY();
void DEC16( uint16_t* );

#endif // INC_16BITARITHMETICGROUP_H 
