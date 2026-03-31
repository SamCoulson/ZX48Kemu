#include "../include/EBTS_group.h"
#include "../include/cpu.h"
#include "../include/memory.h"
#include "../include/util_bit_operations.h"
#include <stdio.h>

// ***EXCHANGE, BLOCK TRANSFER AND SEARCH GROUPS***

// EX DE,HL
// Swap DE and HL register values
// OpCodes: 0xEB
uint8_t EX_DE_HL(Z80 *z80)
{
    EX(z80->de, z80->hl);
    ++*z80->pc;
    return 4;
}

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
void EX(uint16_t *val1, uint16_t *val2)
{
    uint16_t tmp;

    tmp = *val1;
    *val1 = *val2;
    *val2 = tmp;
}

// EXX
// Swap register BC & BC', DE & DE', and HL & HL'
// OpCodes: 0xD9
uint8_t EXX(Z80 *z80)
{
    uint16_t tmp;

    // Swap BC with alt BC
    tmp = *z80->bc;
    *z80->bc = *z80->altbc;
    *z80->altbc = tmp;
    tmp = 0x0000;
    // Swap DE with alt DE
    tmp = *z80->de;
    *z80->de = *z80->altde;
    *z80->altde = tmp;
    tmp = 0x0000;
    // Swap HL with alt HL
    tmp = *z80->hl;
    *z80->hl = *z80->althl;
    *z80->althl = tmp;

    ++*z80->pc;
    return 4;
}

/*
// LDI *Changes flags*
// Copy value at address in HL register to address in DE register
// Increment both HL and DE, and decrement BC
// OpCodes: 0xEDA0
void CPU::LDI(){
        // Copy value at the address stored in HL to the address stored in DE
        uint16_t hlAddrs = byteToWord( &regH, &regL );
        uint16_t deAddrs = byteToWord( &regD, &regE );
        uint8_t hlVal = readByte( hlAddrs );
        writeByte( deAddrs, hlVal );

        // Increment HL and DE
        uint16_t word = byteToWord(&regH, &regL);
        word++;
        regH = getLOByte(&word);
        regL = getHOByte(&word);

        word = byteToWord(&regD, &regE);
        word++;
        regD = getLOByte(&word);
        regE = getHOByte(&word);

        // Decrement BC
        word = byteToWord(&regB, &regC);
        word--;
        regB = getLOByte(&word);
        regC = getHOByte(&word);

        // H flag reset
        setBit(regF, 4, 0 );

        // P/V flag
        // if BC-1 != 0, set 1 else 0
        if( (byteToWord( &regB, &regC)-1) != 0 ){
                setBit(regF, 2, 1 );
        }else{
                setBit(regF, 2, 0 );
        }

        // N flag rest
        setBit(regF, 4, 0 );
}
*/
// LDIR *Changes flags*
// Copy value at address in HL register to address in DE register
// Increment both HL and BC, and decrement BC
// OpCodes: 0xEDB0
uint8_t LDIR(Z80 *z80)
{
    // Reset flags H, P/V, and N
    setBit(z80->f, 4, 0);
    setBit(z80->f, 2, 0);
    setBit(z80->f, 1, 0);

    // Keep copying data from one memory loaction to another until BC is 0
    if (*z80->bc != 0x0000)
    {
        // Transfer byte pointed to by HL to location pointed to by BC
        write_word(*z80->de, read_word_at(*z80->hl));

        // Increment HL and DE
        ++*z80->hl;
        ++*z80->de;

        // Decrement BC
        --*z80->bc;

        --*z80->pc;

        return 5;
    }

    *z80->pc += 2;
    return 4;
}
/*
// LDD *Changes flags*
// Copy value at adress in HL register to address in DE register
// Decrement both HL and BC, and decrement BC
// OpCodes: 0xEDA8
void CPU::LDD(){
        // Copy value at the address stored in HL to the address stored in DE
        uint16_t hlAddrs = byteToWord( &regH, &regL );
        uint16_t deAddrs = byteToWord( &regD, &regE );
        uint8_t hlVal = readByte( hlAddrs );
        writeByte( deAddrs, hlVal );

        // Decrement HL and DE
        uint16_t word = byteToWord(&regH, &regL);
        word--;
        regH = getLOByte(&word);
        regL = getHOByte(&word);

        word = byteToWord(&regD, &regE);
        word--;
        regD = getLOByte(&word);
        regE = getHOByte(&word);

        // Decrement BC
        word = byteToWord(&regB, &regC);
        word--;
        regB = getLOByte(&word);
        regC = getHOByte(&word);

        // H flag reset
        setBit(regF, 4, 0 );

        // P/V flag
        // if BC-1 != 0, set 1 else 0
        if( (byteToWord( &regB, &regC)-1) != 0 ){
                setBit( regF, 2, 1 );
        }else{
                setBit( regF, 2, 0 );
        }

        // N flag reset
        setBit(regF, 4, 0 );
}
*/
// LDDR
// Copy value at adress in HL register to address in DE register
// Decrement both HL and BC.
// OpCodes: 0xEDB8
uint8_t LDDR(Z80 *z80)
{
    // If BC = 0x00 set to 0xFA00 (64) so it can loop over again of 64K
    // if (*z80->bc == 0x0000)
    // {
    //     *z80->bc = 0xFA00;
    // }

    // Reset flags H, P/V, and N
    setBit(z80->f, 4, 0);
    setBit(z80->f, 2, 0);
    setBit(z80->f, 1, 0);

    // Keep copying data from one memory loaction to another until BC is 0
    if (*z80->bc != 0x0000)
    {
        // Transfer byte pointed to by HL to location pointed to by BC
        write_word(*z80->de, read_word_at(*z80->hl));

        // Decrement HL and DE
        --*z80->hl;
        --*z80->de;

        // Decrement BC
        --*z80->bc;

        --*z80->pc;
        return 21;
    }

    *z80->pc += 1;

    return 4;
}
/*
 // CPI *Changes flags*
 // Contents of HL compared with contents of A
 // OpCodes: 0xEDA1
 void CPU::CPI(){
         if( readByte( byteToWord( &regH, &regL ) ) == regA ){
                 setBit( regF, 7, 0 ); // Reset S
                 setBit( regF, 6, 1 ); // Set Z
                 // setBitInByte( regF, ?, ? ); // Set H bit to borrow in bit 4
                 // don't understand this

                 // Increment HL
                 uint16_t hlVal = byteToWord( &regH, &regL );
                 hlVal++;
                 regH = getHOByte( &hlVal );
                 regL = getLOByte( &hlVal );

                 // Decrement BC
                 uint16_t bcVal = byteToWord( &regB, &regC );
                 bcVal++;
                 regH = getHOByte( &bcVal );
                 regL = getLOByte( &bcVal );

         }else{
                 setBit( regF, 7, 1 );
                 setBit( regF, 6, 0 );
                 //See above for H flag
         }

         // P/V flag
         // if BC-1 != 0, set 1 else 0
         if( (byteToWord( &regB, &regC)-1) != 0 ){
                 setBit(regF, 2, 1 );
         }else{
                 setBit(regF, 2, 0 );
         }

         setBit( regF, 1, 1 ); // N is set
 }

 // CPIR *Change flags*
 // Contents of HL compared with contents of A
 // HL and BC affetced by outcome of comparison
 // OpCodes: 0xEDB1
 void CPU::CPIR(){

         // ***THIS WHOLE FUNCTIONS NEEDS CORRECTING POSSIBLY **


         // If BC = 0x00 set to 0xFA00 (64) so it can loop over again of 64K
         if( byteToWord(&regB, &regC) == 0x00){
                 regB = 0xFA;
                 regC = 0x00;
         }


         while( ( byteToWord( &regB, &regC ) > 0x00 ) || (readByte( byteToWord(
 &regH, &regL ) ) != regA  ) ){

                 // Decremnt program counter by 2
                 pc-=2;


                 // Increment HL
                 uint16_t hlVal = byteToWord( &regH, &regL );
                 hlVal++;
                 regH = getHOByte( &hlVal );
                 regL = getLOByte( &hlVal );

                 // Decrement BC
                 uint16_t bcVal = byteToWord( &regB, &regC );
                 bcVal--;
                 regH = getHOByte( &bcVal );
                 regL = getLOByte( &bcVal );

         }

         if( readByte( byteToWord( &regH, &regL ) ) == regA ){
                 setBit( regF, 7, 0 ); // Reset S
                 setBit( regF, 6, 1 ); // Set Z
                 // setBitInByte( regF, ?, ? ); // Set H bit to borrow in bit 4
                 // don't understand this
         }else{
                 setBit( regF, 7, 1 );
                 setBit( regF, 6, 0 );
                         //See above for H flag
         }


         // P/V flag
         // if BC-1 != 0, set 1 else 0
         if( (byteToWord( &regB, &regC)-1) != 0 ){
                 setBit(regF, 2, 1 );
         }else{
                 setBit(regF, 2, 0 );
         }

         setBit( regF, 1, 1 ); // N is set
 }

 // CPD
 // Contents of HL compared with contents of A
 // HL and BC affetced by outcome of comparison
 // OpCodes: 0xEDA9
 void CPU::CPD(){
         if( readByte( byteToWord( &regH, &regL ) ) == regA ){
                 setBit( regF, 7, 0 ); // Reset S
                 setBit( regF, 6, 1 ); // Set Z
                 // setBitInByte( regF, ?, ? ); // Set H bit to borrow in bit 4
                 // don't understand this

                 // Increment HL
                 uint16_t hlVal = byteToWord( &regH, &regL );
                 hlVal--;
                 regH = getHOByte( &hlVal );
                 regL = getLOByte( &hlVal );

                 // Decrement BC
                 uint16_t bcVal = byteToWord( &regB, &regC );
                 bcVal--;
                 regH = getHOByte( &bcVal );
                 regL = getLOByte( &bcVal );

         }else{
                 setBit( regF, 7, 1 );
                 setBit( regF, 6, 0 );
                 //See above for H flag
         }

         // P/V flag
         // if BC-1 != 0, set 1 else 0
         if( (byteToWord( &regB, &regC)-1) != 0 ){
                 setBit(regF, 2, 1 );
         }else{
                 setBit(regF, 2, 0 );
         }

         setBit( regF, 1, 1 ); // N is set

 }

 // CPDR
 // Contents of HL compared with contents of A
 // HL and BC affetced by outcome of comparison
 // OpCodes: 0xEDB9
 void CPU::CPDR(){
 // ***THIS WHOLE FUNCTIONS NEEDS CORRECTING POSSIBLY **


         // If BC = 0x00 set to 0xFA00 (64) so it can loop over again of 64K
         if( byteToWord(&regB, &regC) == 0x00){
                 regB = 0xFA;
                 regC = 0x00;
         }


         while( ( byteToWord( &regB, &regC ) > 0x00 ) || (readByte( byteToWord(
 &regH, &regL ) ) != regA  ) ){

                 // Decremnt program counter by 2
                 pc-=2;


                 // Decrement HL
                 uint16_t hlVal = byteToWord( &regH, &regL );
                 hlVal--;
                 regH = getHOByte( &hlVal );
                 regL = getLOByte( &hlVal );

                 // Decrement BC
                 uint16_t bcVal = byteToWord( &regB, &regC );
                 bcVal--;
                 regH = getHOByte( &bcVal );
                 regL = getLOByte( &bcVal );

         }

         if( readByte( byteToWord( &regH, &regL ) ) == regA ){
                 setBit( regF, 7, 0 ); // Reset S
                 setBit( regF, 6, 1 ); // Set Z
                 // setBitInByte( regF, ?, ? ); // Set H bit to borrow in bit 4
                 // don't understand this
         }else{
                 setBit( regF, 7, 1 );
                 setBit( regF, 6, 0 );
                         //See above for H flag
         }


         // P/V flag
         // if BC-1 != 0, set 1 else 0
         if( (byteToWord( &regB, &regC)-1) != 0 ){
                 setBit(regF, 2, 1 );
         }else{
                 setBit(regF, 2, 0 );
         }

         setBit( regF, 1, 1 ); // N is set
 }
 */
