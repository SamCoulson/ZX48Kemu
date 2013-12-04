# taken from http://stackoverflow.com/questions/1484817/how-do-i-make-a-simple-makefile-gcc

CC=gcc

CFLAGS=-Wall -std=c99 

LDFLAGS=-I ./include/ 

TARGET=-o z80test 

SDIR=src

ODIR=obj
	  
z80 : z80test.o ../src/util_bit_operations.o ../src/8-bit_arithmetic_group.o 
	${CC} ${CFLAGS} ${LDFLAGS} ${TARGET} z80test.o ../src/8-bit_arithmetic_group.o ../src/util_bit_operations.o  
clean:
	-del $(SDIR)\*.o
	-del z80test
