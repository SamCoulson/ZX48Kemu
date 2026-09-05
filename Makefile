# taken from http://stackoverflow.com/questions/1484817/how-do-i-make-a-simple-makefile-gcc

CC=gcc
CPPFLAGS = -Iinclude -D_POSIX_C_SOURCE=199309L
CFLAGS= -O0 -Wall -std=c99
LDFLAGS= -L./lib 
LDLIBS= -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

SOURCES := $(wildcard src/*.c)
OBJECTS := $(SOURCES:.c=.o)

SDIR=./src

z80 : ${OBJECTS}
	${CC} ${LDFLAGS} -o $@ $^ ${LDLIBS} 

clean:
	-rm $(SDIR)/*.o
	-rm z80
