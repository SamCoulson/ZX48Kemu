# taken from http://stackoverflow.com/questions/1484817/how-do-i-make-a-simple-makefile-gcc

CC=gcc

CFLAGS=-Wall -std=c99 

LDFLAGS=-I ./include/ 

TARGET=-o z80 

SDIR=src

ODIR=obj
	  
z80 : src/main.o src/console.o src/romloader.o src/memory.o src/cpu.o src/8-bit_load_group.o src/GPA_and_CPU_control_group.o \
	src/util_bit_operations.o src/16-bit_load_group.o src/jump_group.o src/8-bit_arithmetic_group.o \
	src/IO_group.o src/16-bit_arithmetic_group.o src/EBTS_group.o src/BSR_and_test_group.c \
	src/call_and_return_group.o src/rotate_and_shift_group.o src/ULA.o src/screen.o
	${CC} ${CFLAGS} ${LDFLAGS} ${TARGET} src/main.o src/console.o src/romloader.o src/memory.o src/cpu.o src/8-bit_load_group.o \
		src/GPA_and_CPU_control_group.o src/util_bit_operations.o src/16-bit_load_group.o src/jump_group.o \
		src/8-bit_arithmetic_group.o src/IO_group.o src/16-bit_arithmetic_group.o src/EBTS_group.o \
		src/BSR_and_test_group.c src/call_and_return_group.o src/rotate_and_shift_group.o src/ULA.o \
		src/screen.o  -lmingw32 -lSDLmain -lSDL -mconsole 
clean:
	-del $(SDIR)\*.o
	-del z80
