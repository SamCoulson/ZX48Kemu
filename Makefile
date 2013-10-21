CXX=g++

CXXFLAGS=-Wall

LDFLAGS=-I ./include/

TARGET=-o z80

z80 : src/main.o src/memory.o src/cpu.o src/8-bit_load_groups.o src/16-bit_load_group.o \
       	src/EBTS_group.o src/romloader.o  	
	${CXX} ${CXXFLAGS} ${LDFLAGS} ${TARGET} src/main.o src/memory.o src/cpu.o src/8-bit_load_groups.o \
		src/16-bit_load_group.o src/EBTS_group.cpp src/romloader.o

main.o : src/main.cpp
	${CXX} ${CXXFLAGS} ${LDFLAGS} $<

memory.o : src/memory.cpp
	${CXX} ${CXXFLAGS} ${LDFLAGS} $<

cpu.o : src/cpu.cpp
	${CXX} ${CXXFLAGS} ${LDFLAGS} $<

8-bit_load_groups.o : src/8-bit_load_groups.cpp
	${CXX} ${CXXFLAGS} ${LDFLAGS} $<

16-bit_load_group.o : src/16-bit_load_group.cpp
	${CXX} ${CXXFLAGS} ${LDFLAGS} $<

ebts_group.o : src/EBTS_group.cpp
	${CXX} ${CXXFLAGS} ${LDFLAGS} $<

romloader.o : src/romloader.cpp 
	${CXX} ${CXXFLAGS} ${LDFLAGS} $<


