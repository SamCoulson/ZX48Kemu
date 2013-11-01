# taken from http://stackoverflow.com/questions/1484817/how-do-i-make-a-simple-makefile-gcc

CC=gcc

CFLAGS=-Wall -std=c99

LDFLAGS=-I ./include/

TARGET=-o z80

SDIR=src

ODIR=obj
	  
z80 : src/main.o src/romloader.o src/memory.o src/cpu.o
	${CC} ${CFLAGS} ${LDFLAGS} ${TARGET} src/main.o src/romloader.o src/memory.o src/cpu.o

clean:
	-del $(SDIR)\*.o
	-del z80
