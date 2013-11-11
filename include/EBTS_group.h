#ifndef INC_EBTS_H
#define INC_EBTS_H 

// ***EXCHANGE, BLOCK TRANSFER AND SEARCH GROUPS***

// EX DE,HL
// Swap DE and HL register values
// OpCodes: 0xEB

// EX AF,AF'
// Swap register values between AF and alternative AF 
// OpCodes: 0x08

// EX (SP),HL
// Swap values between two HL register and 2-bytes on stack at SP
// OpCodes: 0xE3

// EX (SP),IX
// Swap values between two HL register and 2-bytes on stack at SP
// OpCodes: 0xDDE3

// EX (SP),IY
// Swap values between two IX register and 2-bytes on stack at SP 
// OpCodes: 0xFDE3
void EX( uint16_t* val1, uint16_t* val2 );

// EXX
// Swap register BC & BC', DE & DE', and HL & HL'
// OpCodes: 0xD9
void EXX( uint16_t* bc, uint16_t* de, uint16_t* hl, uint16_t* altbc, uint16_t* altde, uint16_t* althl );


/*
// LDI *Changes flags*
// Copy value at adress in HL register to address in DE register
// Increment both HL and BC, and decrement BC
// OpCodes: 0xEDA0
void LDI();
*/
// LDIR *Changes flags*
// Copy value at adress in HL register to address in DE register
// Increment both HL and BC, and decrement BC
// OpCodes: 0xEDB0
void LDIR( uint16_t* hlVal, uint16_t* deVal, uint16_t* hl, uint16_t* de, uint16_t* bc, uint8_t* fReg );

/*
// LDD *Changes flags* 
// Copy value at adress in HL register to address in DE register
// Decrement both HL and BC, and decrement BC
// OpCodes: 0xEDA8
void LDD();

*/
// LDDR
// Copy value at adress in HL register to address in DE register
// Decrement both HL and BC, and decrement BC
// OpCodes: 0xEDB8
void LDDR( uint16_t* hlVal, uint16_t* deVal, uint16_t* hl, uint16_t* de, uint16_t* bc, uint8_t* fReg );

/*
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
*/
#endif // INC_EBTS_H`
