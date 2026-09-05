// Unit tests for z80 emu

#include "16-bit_arithmetic_group.h"
#include "8-bit_arithmetic_group.h"
#include "cpu.h"
#include "util_bit_operations.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// Allows the CPU z80isters to be zero'd out
int resetCPU()
{
    // Initialise z80isters to starting values, this can be used for testing
    *z80->af = 0x0000;
    *z80->bc = 0x0000;
    *z80->de = 0x0000;
    *z80->hl = 0x0000;

    *z80->altaf = 0x0000;
    *z80->altbc = 0x0000;
    *z80->altde = 0x0000;
    *z80->althl = 0x0000;

    *z80->ir = 0x00;
    *z80->ix = 0x00;
    *z80->iy = 0x00;

    *z80->sp = 0x00;
    *z80->pc = 0x00;

    *z80->iff1 = 0x00;
    *z80->iff2 = 0x00;

    return 0;
}

static void test_bit_manipulation()
{

    // Test Bit
    resetCPU();
    // Signed bit is 0
    uint8_t foo = 0x0;
    printf("Signed bit = %d\n", (foo & (1 << 7)));
    foo |= (1 << 7);
    printf("Signed bit = %d\n", (foo & (1 << 7)));
}

static void test_8bit_add()
{

    // Test Carry flag
    resetCPU();
    *z80->a = 128;
    *z80->b = 128;
    ADD(z80->a, z80->b, z80->f);
    assert(*z80->a == 0 && getBit(z80->f, 0) == 0x01 &&
           "test_8bit_add() - Carry Flag");

    // Test N(Addition/Subtraction) flag
    resetCPU();
    *z80->a = 1;
    *z80->b = 1;
    ADD(z80->a, z80->b, z80->f);
    assert(*z80->a == 2 && getBit(z80->f, 1) == 0x00 &&
           "test_8bit_add() - N Flag");

    // Test P/V flag (parity / overflow)
    resetCPU();
    *z80->a = 64; // 0100 0000
    *z80->b = 64; // 0100 0000
    ADD(z80->a, z80->b, z80->f);
    assert(*z80->a == 128 && getBit(z80->f, 2) == 0x01 &&
           "test_8bit_add() - P/V Flag");

    // Test H (Half carry) flag
    // resetCPU();
    //*z80->a = 4;
    //*z80->b = 4;
    // ADD( z80->a, z80->b, z80->f );
    // assert( *z80->a == 8 && getBit( z80->f, 4 ) == 0x01  && "test_8bit_add()
    // - H Flag" );

    const uint8_t values[6] = {0, 1, 127, 128, 129, 255};

    resetCPU();
    for (int i = 0; i < sizeof(values); i++)
    {

        *z80->a = values[i];
        printf("Uint Sint  Uint Sint\n");

        for (int j = 0; j < sizeof(values); j++)
        {

            *z80->b = values[j];

            printf("%3d(%4d) + %3d(%4d) = ", *z80->a, (int8_t)*z80->a, *z80->b,
                   (int8_t)*z80->b);

            ADD(z80->a, z80->b, z80->f);

            printf(" %3d(%4d) C = %d V = %d\n", *z80->a, (int8_t)*z80->a,
                   getBit(z80->f, 0), getBit(z80->f, 2));
            // Reset a
            *z80->a = values[i];
        }
    }
}

static void test_8bit_sub()
{
    // Test Carry flag
    resetCPU();
    *z80->a = 0;
    *z80->b = 2;
    SUB(z80->a, z80->b, z80->f);
    assert((int8_t)*z80->a < 0 && getBit(z80->f, 0) == 0x01 &&
           "test_8bit_sub() - Carry Flag");

    // Test N(Addition/Subtraction) flag
    resetCPU();
    *z80->a = 1;
    *z80->b = 1;
    SUB(z80->a, z80->b, z80->f);
    assert(*z80->a == 0 && getBit(z80->f, 1) == 0x01 &&
           "test_8bit_sub() - N Flag");

    // Test P/V flag (parity / overflow)
    resetCPU();
    *z80->a = 128; // 0100 0000
    *z80->b = 128; // 0100 0000
    SUB(z80->a, z80->b, z80->f);
    assert(*z80->a == 0 && getBit(z80->f, 2) == 0x01 &&
           "test_8bit_sub() - P/V Flag");

    // Test H (Half carry) flag
    // resetCPU();
    //*z80->a = 4;
    //*z80->b = 4;
    // ADD( z80->a, z80->b, z80->f );
    // assert( *z80->a == 8 && getBit( z80->f, 4 ) == 0x01  && "test_8bit_add()
    // - H Flag" );

    const uint8_t values[6] = {0, 1, 127, 128, 129, 255};

    resetCPU();
    for (int i = 0; i < sizeof(values); i++)
    {

        *z80->a = values[i];
        printf("Uint Sint  Uint Sint\n");

        for (int j = 0; j < sizeof(values); j++)
        {

            *z80->b = values[j];

            printf("%3d(%4d) - %3d(%4d) = ", *z80->a, (int8_t)*z80->a, *z80->b,
                   (int8_t)*z80->b);

            SUB(z80->a, z80->b, z80->f);

            printf(" %3d(%4d) C = %d V = %d\n", *z80->a, (int8_t)*z80->a,
                   getBit(z80->f, 0), getBit(z80->f, 2));
            // Reset a
            *z80->a = values[i];
        }
    }
}

static void test_16bit_add()
{
    // Test carry flag
    resetCPU();
    *z80->bc = 32768;
    *z80->de = 32768;
    printf("%d + %d", *z80->bc, *z80->de);
    ADD16(z80->bc, z80->de, z80->f);
    printf(" = %d\n", *z80->bc);
    printf("Carry flag = %d\n", getBit(z80->f, 0));
    assert(*z80->bc == 0x00 && getBit(z80->f, 0) == 0x01 &&
           "test_16bit_sub() - Carry Flag");

    // test half carry
    resetCPU();
    *z80->bc = 128;
    *z80->de = 128;
    printf("%d + %d", *z80->bc, *z80->de);
    ADD16(z80->bc, z80->de, z80->f);
    printf(" = %d\n", *z80->bc);
    printf("Half Carry flag = %d\n", getBit(z80->f, 4));
    assert(*z80->bc == 256 && getBit(z80->f, 4) == 0x01 &&
           "test_16bit_sub() - Carry Flag");
}

int main(int argc, char *argv[])
{

    printf("Testing started!\n");
    // test_bit_manipulation();

    printf("Testing 8Bit ADD\n");
    test_8bit_add();
    printf("Testing 8Bit ADD successful\n");

    printf("Testing 8Bit SUB\n");
    test_8bit_sub();
    printf("Testing 8Bit SUB successful\n");

    printf("Testing 16Bit ADD16\n");
    test_16bit_add();
    // printf("Testing 8Bit SUB successful\n");

    printf("Testing successful");

    return 0;
}
