#include "../include/cpu.h"
#include <assert.h>
#include <stdio.h>
#include <conio.h>
//#include "../include/GPA_and_CPU_control_group.h"
//#include "../include/16-bit_load_group.h"
//#include "../include/16-bit_arithmetic_group.h"

#define BUF_SIZE 5

// Declare and initialise structure of pointers to register unions, this must be here not in the .h file
static Registers registers = {
        // Assign const pointers to main registers	
	.a = &af._af[0], .f = &af._af[1], .af = &af.af,
	.b = &bc._bc[0], .c = &bc._bc[1], .bc = &bc.bc,
	.d = &de._de[0], .e = &de._de[1], .de = &de.de,
	.h = &hl._hl[0], .l = &hl._hl[1], .hl = &hl.hl,
	// Assign const pointers to alternative registers 
	.alta = &altaf._af[0], .altf = &altaf._af[1], .altaf = &altaf.af,
	.altb = &altbc._bc[0], .altc = &altbc._bc[1], .altbc = &altbc.bc,
	.altd = &altde._de[0], .alte = &altde._de[1], .altde = &altde.de,
	.alth = &althl._hl[0], .altl = &althl._hl[1], .althl = &althl.hl,
	// 8-bit Special registers - interrupt vector
	.i = &ir._ir[0], .r = &ir._ir[1], .ir = &ir.ir,
	// 16-bit index registers
	.ix = &ix, .iy = &iy,
      	// 16-bit stack pointer and program pointer
	.sp = &sp, .pc = &pc,
	// IFF (Interrupt enabled flip-flop)
	.iff1 = &iff1, .iff2 = &iff2 	
};

// Pointer to the register structure
Registers* reg = &registers;


// Static structure for registers
int initCPU(){


	*reg->b = 0xFF;
	*reg->c = 0xEE;


	printf("%X%X\n", *reg->b, *reg->c);

	return 0;
}
//  Set the PC to point to a location in memory


void execute( uint8_t* opcode ){	

	printf("%04x\t",pc);

	switch( *opcode ){
		case 0x00:
			//printf( "NOP (Not implemented yet!");
			//NOP();
			break;
		case 0x01:
			//LD16BitConstTo16BitReg(regB, regC, readByte(++pc), readByte(pc++) );
			////printf( "LD BC,%d%d", (int)regB, (int)regC );
			break;
		case 0x02:
			//printf( "LD(BC),A" );
			//LDAToAddrsOfBC();
			break;/*
		case 0x03:
			//printf( "JR NC,";
		break;*/
		case 0x04:
			//printf( "INC B" );
			//INC8BitReg( regB );
			break;/*
		case 0x05:
			break;*/
		case 0x06:
			//printf( "LD B,%d", readByte(++pc) );
			//LD8BitIntTo8BitReg( regB, readByte(pc) );
			break;/*
		case 0x07:
			break;*/
		case 0x08:
			//printf( "EX AF,AF" );
			//EXAFAltAF();
			break;/*
		case 0x09:
			break;*/
		case 0x0A:
			//printf( "LD A,(BC)" );
			//LDAddrsOfBCToA();
			break;/*
		case 0x0B:
			break;
		case 0x0C:
			break;
		case 0x0D:
			break;*/
		case 0x0E:
			//printf( "LD C,%d", (unsigned int)readByte(++pc) );
			//LD8BitIntTo8BitReg( regC, readByte(pc) );
			break;/*
		case 0x0F:
			break;
		case 0x10:
			break;*/
		case 0x11:
			//LD16BitConstTo16BitReg(regD, regE, readByte(++pc), readByte(++pc) );
			//printf( "LD DE,%d%d", (int)regD, (int)regE );
			break;
		case 0x12:
			//printf( "LD(DE),A");
			//LDAToAddrsOfDE();
			break;/*
		case 0x13:
			break;
		case 0x14:
			break;*/
		case 0x15:
			//printf( "DEC D" );
			//DEC8BitReg( regD );
			break;
		case 0x16:
			//printf( "LD D,%d",(int)readByte(++pc) );
			//LD8BitIntTo8BitReg( regD, readByte(pc) );
			break;/*
		case 0x17:
			break;
		case 0x18:
			//printf( "JR 00%X";
			break;*/
		case 0x19:
			//printf( "ADD HL,DE");
			//ADD16BitRegToHL( regD, regE );
			break;
		case 0x1A:
			//printf( "LD A,(DE)" );
			//LDAddrsOfDEToA();
			break;
		case 0x1B:/*
			break;
		case 0x1C:
			break;
		case 0x1D:
			break;*/
		case 0x1E:
			//printf( "LD E,%d", readByte(++pc) );
			//LD8BitIntTo8BitReg( regE, readByte(pc) );
			break;/*
		case 0x1F:
			break;*/
		case 0x20:
			//printf( "JR NZ,%d", (int)(int8_t)readByte( pc+1 ) );
			//JRNZ( (int8_t)readByte( ++pc ) );
			break;
		case 0x21:
			//LD16BitConstTo16BitReg(regH, regL, readByte(++pc), readByte(pc++) );
			//printf( "LD HL,%d",(int)regH << (int)regL );
			break;
		case 0x22:
			//printf( "LD (%d%d)", (int)readByte( pc+1 ), (int)readByte( pc+2 ) );
			//LDHLRegToAddrsOf16BitInt( readByte( ++pc ), readByte( ++pc ) );							
			break;
		case 0x23:
			//printf( "INC HL" );
			//INC16BitReg( regH, regL );
			break;/*
		case 0x24:
			break;
		case 0x25:
			break;*/
		case 0x26:
			//printf( "LD H,%d",readByte(++pc) );
			//LD8BitIntTo8BitReg( regH, readByte(pc) );
			break;/*
		case 0x27:
			break;*/
		case 0x28:
			//printf( "JR Z, %d", readByte( pc+2 ) );
			//JRZ( readByte( ++pc ) );
			break;/*
		case 0x29:
			break;*/
		case 0x2A:
			//printf( "LD HL,(%d%d)", (int)readByte( pc+1 ), (int)readByte( pc+2 ) ); 
			//LDAddrsOf16BitIntToHLReg( readByte( ++pc ), readByte( ++pc )  );
			break;
		case 0x2B:
			//printf( "DEC HL");
			//DEC16BitReg( regH, regL);
			break;/*
		case 0x2C:
			break;
		case 0x2D:
			break;*/
		case 0x2E:
			//printf( "LD L,%d",readByte(++pc) );
			//LD8BitIntTo8BitReg( regL, readByte(pc) );
			break;/*
		case 0x2F:
			break;*/
		case 0x30:
			//printf("JR NC,%d", readByte( pc+1 ) );
			//JRNC( (int)readByte( ++pc ) );
			break;/*
		case 0x31:
			LD(regS, regP, readByte(pc+=2), readByte(pc-1) );
			//printf( "LD SP,"<< std::hex << (int)regH << (int)regL;
			break;*/
		case 0x32:
			//printf( "LD (%d%d),A", (unsigned int)readByte(pc+1), (unsigned int)readByte(pc+2) );
			//LDAToAddrsOf16BitInt( readByte(++pc), readByte(++pc) );
			break;/*
		case 0x33:
			break;
		case 0x34:
			break;*/
		case 0x35:
			//printf("DEC HL");
			//DECAddrsOfHL();
			break;
		case 0x36:
			//printf( "LD (HL),+%d",(int)readByte( pc+1 ) );
			//LD8BitIntToAddrsOfHL( readByte( ++pc ) );
			break;/*
		case 0x37:
			//printf( "SCF";
		break;
		case 0x38:
			//printf( "JR C,%d";
		break;
		case 0x39:
			//printf("");
			break;*/
		case 0x3A:
			//printf( "LD A,(%d%d)",(int)readByte( pc+1 ), (int)readByte( pc+2 ) );
			//LDAddrsOf16BitIntToA( regA, readByte( ++pc ), readByte( ++pc ) );
			break;/*
		case 0x3B:
			//printf("");
			break;
		case 0x3C:
			//printf("");
			break;
		case 0x3D:
			//printf( "DEC A";
		break;*/
		case 0x3E:
			//printf( "LD A,%d",(int)readByte(pc+1) );
			//LD8BitIntTo8BitReg( regA, readByte(++pc) );
		break;/*
		case 0x3F:
			//printf( "CCF";
		break;*/
		case 0x40:
			//printf( "LD B,B" );
			//LD8BitRegTo8BitReg( regB, regB );
			break;
		case 0x41:
			//printf( "LD B,C" );
			//LD8BitRegTo8BitReg( regB, regC );
			break;
		case 0x42:
			//printf( "LD B,D" );
			//LD8BitRegTo8BitReg( regB, regD );
			break;
		case 0x43:
			//printf( "LD B,E" );
			//LD8BitRegTo8BitReg( regB, regE );
			break;
		case 0x44:
			//printf( "LD B,H" );
			//LD8BitRegTo8BitReg( regB, regH );
			break;
		case 0x45:
			//printf( "LD B,L" );
			//LD8BitRegTo8BitReg( regB, regL );
			break;
		case 0x46:
			//printf( "LD B,(HL)");
			//LDAddrsOfHLTo8BitReg( regB );
			break;
		case 0x47:
			//printf( "LD B,A");
			//LD8BitRegTo8BitReg( regB, regA );
			break;
		case 0x48:
			//printf( "LD C,B");
			//LD8BitRegTo8BitReg( regB, regB );
			break;
		case 0x49:
			//printf( "LD C,C");
			//LD8BitRegTo8BitReg( regB, regC );
			break;
		case 0x4A:
			//printf( "LD C,D");
			//LD8BitRegTo8BitReg( regB, regD );
			break;
		case 0x4B:
			//printf( "LD C,E");
			//LD8BitRegTo8BitReg( regB, regE );
			break;
		case 0x4C:
			//printf( "LD C,H");
			//LD8BitRegTo8BitReg( regB, regH );
			break;
		case 0x4D:
			//printf( "LD C,L");
			//LD8BitRegTo8BitReg( regB, regL );
			break;
		case 0x4E:
			//printf( "LD C,(HL)");
			//LDAddrsOfHLTo8BitReg( regC );
			break;
		case 0x4F:
			//printf( "LD C,A");
			//LD8BitRegTo8BitReg( regB, regA );
			break;
		case 0x50:
			//printf( "LD D,B");
			//LD8BitRegTo8BitReg( regD, regB );
			break;
		case 0x51:
			//printf( "LD D,C");
			//LD8BitRegTo8BitReg( regD, regC );
			break;
		case 0x52:
			//printf( "LD D,D");
			//LD8BitRegTo8BitReg( regD, regD );
			break;
		case 0x53:
			//printf( "LD D,E");
			//LD8BitRegTo8BitReg( regD, regE );
			break;
		case 0x54:
			//printf( "LD D,H");
			//LD8BitRegTo8BitReg( regD, regH );
			break;
		case 0x55:
			//printf( "LD D,L");
			//LD8BitRegTo8BitReg( regD, regL );
			break;
		case 0x56:
			//printf( "LD D,(HL)");
			//LDAddrsOfHLTo8BitReg( regB );
			break;
		case 0x57:
			//printf( "LD E,A");
			//LD8BitRegTo8BitReg( regD, regA );
			break;
		case 0x58:
			//printf( "LD E,B");
			//LD8BitRegTo8BitReg(regE, regB);
			break;
		case 0x59:
			//printf( "LD E,C");
			//LD8BitRegTo8BitReg(regE, regC);
			break;
		case 0x5A:
			//printf( "LD E,D");
			//LD8BitRegTo8BitReg(regE, regD);
			break;
		case 0x5B:
			//printf( "LD E,E");
			//LD8BitRegTo8BitReg(regE, regE);
			break;
		case 0x5C:
			//printf( "LD E,H");
			//LD8BitRegTo8BitReg(regE, regH);
			break;
		case 0x5D:
			//printf( "LD E,L");
			//LD8BitRegTo8BitReg(regE, regL);
			break;
		case 0x5E:
			//printf( "LD E,(HL)");
			//LDAddrsOfHLTo8BitReg( regE );
			break;
		case 0x5F:
			//printf( "LD D,A");
			//LD8BitRegTo8BitReg(regD, regA);
			break;
		case 0x60:
			//printf( "LD H,B");
			//LD8BitRegTo8BitReg(regH, regB);
			break;
		case 0x61:
			//printf( "LD H,C");
			//LD8BitRegTo8BitReg(regH, regC);
			break;
		case 0x62:
			//printf( "LD H,D");
			//LD8BitRegTo8BitReg(regH, regD);
			break;
		case 0x63:
			//printf( "LD H,E");
			//LD8BitRegTo8BitReg(regH, regE);
			break;
		case 0x64:
			//printf( "LD H,H");
			//LD8BitRegTo8BitReg(regH, regH);
			break;
		case 0x65:
			//printf( "LD H,L");
			//LD8BitRegTo8BitReg(regH, regL);
			break;
		case 0x66:
			//printf( "LD H,(HL)");
			//LDAddrsOfHLTo8BitReg( regH );
			break;
		case 0x67:
			//printf( "LD H,A");
			//LD8BitRegTo8BitReg(regH, regA);
			break;
		case 0x68:
			//printf( "LD L,B");
			//LD8BitRegTo8BitReg(regL, regB);
			break;
		case 0x69:
			//printf( "LD L,C");
			//LD8BitRegTo8BitReg(regL, regC);
			break;
		case 0x6A:
			//printf( "LD L,D");
			//LD8BitRegTo8BitReg(regL, regD);
			break;
		case 0x6B:
			//printf( "LD L,E");
			//LD8BitRegTo8BitReg(regL, regE);
			break;
		case 0x6C:
			//printf( "LD L,H");
			//LD8BitRegTo8BitReg(regL, regH);
			break;
		case 0x6D:
			//printf( "LD L,L");
			//LD8BitRegTo8BitReg(regL, regL);
			break;
		case 0x6E:
			//printf( "LD L,(HL)");
			//LDAddrsOfHLTo8BitReg( regL );
			break;
		case 0x6F:
			//printf( "LD L,A");
			//LD8BitRegTo8BitReg(regL, regA);
			break;
		case 0x70:
			//printf( "(HL),B");
			//LD8BitRegToAddrsOfHL(regB);
			break;
		case 0x71:
			//printf( "(HL),C");
			//LD8BitRegToAddrsOfHL(regC);
			break;
		case 0x72:
			//printf( "(HL),D");
			//LD8BitRegToAddrsOfHL(regD);
			break;
		case 0x73:
			//printf( "(HL),E");
			//LD8BitRegToAddrsOfHL(regE);
			break;
		case 0x74:
			//printf( "(HL),H");
			//LD8BitRegToAddrsOfHL(regH);
			break;
		case 0x75:
			//printf( "(HL),L");
			//LD8BitRegToAddrsOfHL(regL);
			break;/*
		case 0x76:
			cout << "HALT");
		break;*/
		case 0x77:
			//printf( "(HL),A");
			//LD8BitRegToAddrsOfHL(regA);
			break;
		case 0x78:
			//printf( "LD A,B");
			//LD8BitRegTo8BitReg(regA, regB);
			break;
		case 0x79:
			//printf( "LD A,C");
			//LD8BitRegTo8BitReg(regA, regC);
			break;
		case 0x7A:
			//printf( "LD A,D");
			//LD8BitRegTo8BitReg(regA, regD);
			break;
		case 0x7B:
			//printf( "LD A,E");
			//LD8BitRegTo8BitReg(regA, regE);
			break;
		case 0x7C:
			//printf( "LD A,H");
			//LD8BitRegTo8BitReg(regA, regH);
			break;
		case 0x7D:
			//printf( "LD A,L");
			//LD8BitRegTo8BitReg(regA, regL);
			break;
		case 0x7E:
			//printf( "LD A,(HL)");
			//LDAddrsOfHLTo8BitReg( regA );
			break;
		case 0x7F:
			//printf( "LD A,A");
			//LD8BitRegTo8BitReg(regA, regA);
			break;/*
		case 0x80:
			break;
		case 0x81:
			break;
		case 0x82:
			break;
		case 0x83:
			break;
		case 0x84:
			break;
		case 0x85:
			break;
		case 0x86:
			break;
		case 0x87:
			break;
		case 0x88:
			break;
		case 0x89:
			break;
		case 0x8A:
			break;
		case 0x8B:
			break;
		case 0x8C:
			break;
		case 0x8D:
			break;
		case 0x8E:
			break;
		case 0x8F:
			break;
		case 0x90:
			break;
		case 0x91:
			break;
		case 0x92:
			break;
		case 0x93:
			break;
		case 0x94:
			break;
		case 0x95:
			break;
		case 0x96:
			break;
		case 0x97:
			break;
		case 0x98:
			break;
		case 0x99:
			break;
		case 0x9A:
			break;
		case 0x9B:
			break;
		case 0x9C:
			break;
		case 0x9D:
			break;
		case 0x9E:
			break;
		case 0x9F:
			break;
		case 0xA0:
			break;
		case 0xA1:
			break;
		case 0xA2:
			break;
		case 0xA3:
			break;
		case 0xA4:
			break;
		case 0xA5:
			break;
		case 0xA6:
			break;*/
		case 0xA7:
			//printf( "AND A" );
			//AND8BitValWithA( regA );
			break;/*
		case 0xA8:
			break;
		case 0xA9:
			break;
		case 0xAA:
			break;
		case 0xAB:
			break;
		case 0xAC:
			break;
		case 0xAD:
			break;
		case 0xAE:
			break;*/
		case 0xAF:
			//printf( "XOR A");
			//XOR8BitValWithA( regA );
			break;/*
		case 0xB0:
			break;
		case 0xB1:
			break;
		case 0xB2:
			break;
		case 0xB3:
			break;
		case 0xB4:
			break;
		case 0xB5:
			//printf( "OR L";
			break;
		case 0xB6:
			break;
		case 0xB7:
			break;
		case 0xB8:
			break;
		case 0xB9:
			break;
		case 0xBA:
			break;
		case 0xBB:
			break;*/
		case 0xBC:
			//printf( "CP H");
			//CP8BitValWithA( regH );
			break;/*
		case 0xBD:
			break;
		case 0xBE:
			break;
		case 0xBF:
			//printf( "CP A";
		break;
		case 0xC0:
			//printf( "RET NZ";
		break;*/
		case 0xC1:
			//printf( "POP BC");
			//POP( regB, regC );
			break;
		case 0xC2:
			//printf( "JP NZ %d%d",(int)readByte( pc+2 ), (int)readByte( pc+1 ) );
			//JPCondition( "NZ", readByte( ++pc ), readByte( ++pc ) );
			break;
		case 0xC3:
			//printf( "JP %d%d", (int)readByte( pc+2 ), (int)readByte( pc+1 ) );
			//JP( readByte( ++pc ), readByte( ++pc ) );
		break;/*
		case 0xC4:
			break;*/
		case 0xC5:
			//printf( "PUSH BC");
			//PUSH( regB, regC );
			break;
		break;/*
		case 0xC6:
			break;
		case 0xC7:
			break;
		case 0xC8:
			//printf( "RET Z");
		break;
		case 0xC9:
			//printf( "RET");
		break;
		case 0xCA:
			break;
		case 0xCB:
			break;
		case 0xCC:
			break;
		case 0xCD:
			//printf( "CALL %X%X");
		break;
		case 0xCE:
			break;
		case 0xCF:
			break;
		case 0xD0:
			//printf( "RET NC");
		break;*/
		case 0xD1:
			//printf( "POP DE");
			//POP( regD, regE );
			break;/*
		case 0xD2:
			break;*/
		case 0xD3:
			//printf("OUT (%d),A", (int)readByte( pc+1 ));
			//OUTA( readByte( ++pc ) );
			break;/*
		case 0xD4:
			//printf( "CALL NC,%X%X");
			break;*/
		case 0xD5:
			//printf( "PUSH DE");
			//PUSH( regD, regE );
			break;/*
		case 0xD6:
			break;
		case 0xD7:
			break;
		case 0xD8:
			//printf( "RET C");
			break;*/
		case 0xD9:
			//printf( "EXX");
			//EXX();
			break;/*
		case 0xDA:
			break;
		case 0xDB:
			break;
		case 0xDC:
			break;*/
		case 0xDD:
			switch( (int)readByte( ++pc ) ){/*
			case 0x21:
				//printf( "LD IX," << (int)readByte( pc+1 ) << (int)readByte(pc+2);
				LD16BitIntToIXReg( readByte(++pc), readByte(++pc) );
				break;
			case 0x22:
				//printf( "LD (" << (int)readByte( pc+1 ) << (int)readByte(pc+2) << "),IX");
				LDIXRegToAddrsOf16BitInt( readByte( ++pc ), readByte( ++pc ) );	
				break;
			case 0x2A:
				//printf( "LD IX(" << (int)readByte( pc+1 ) << (int)readByte( pc+2 ) << ")");
				LD16BitAddrsOf16BitIntToIX( readByte( ++pc ), readByte( ++pc ) );
				break;
			case 0x35:
				//printf( "DEC IX +%d", readByte( pc+1 ) );
				DECAddrsOfIXOffset( readByte( ++pc ) );
				break;
			case 0x36:
				//printf( "LD (IX+"<< (int)readByte(pc+1) << ")" << (int)readByte(pc+2);
				LD8BitIntToAddrsOfIXd( readByte(++pc), readByte(++pc) );
				break;
			case 0x46:
				//printf( "LD B,(IX+" << (int)readByte( pc+1 ) << ")");
				LDAddrsOfIXdTo8BitReg(regB, readByte(++pc) ); 
				break;
			case 0x4E:
				//printf( "LD C,(IX+" << (int)readByte( pc+1 ) << ")");
				LDAddrsOfIXdTo8BitReg(regC, readByte(++pc) );
				break;
			case 0x5E:
				//printf( "LD D,(IX+" << (int)readByte( pc+1 ) << ")");
				LDAddrsOfIXdTo8BitReg(regD, readByte(++pc) );
				break;
			case 0x56:
				//printf( "LD E,(IX+" << (int)readByte( pc+1 ) << ")");
				LDAddrsOfIXdTo8BitReg(regE, readByte(++pc) );
				break;
			case 0x66:
				//printf( "LD H,(IX+" << (int)readByte( pc+1 ) << ")");
				LDAddrsOfIXdTo8BitReg(regH, readByte(++pc) );
				break;
			case 0x6E:
				//printf( "LD L,(IX+" << (int)readByte( pc+1 ) << ")");
				LDAddrsOfIXdTo8BitReg( regL, readByte(++pc) );
				break;
			case 0x70:
				//printf( "LD (IX+" << (int)readByte( pc+1 ) << "),B");
				LD8BitRegToAddrsOfIXd( regB,readByte( ++pc ) );
				break;
			case 0x71:
				//printf( "LD (IX+" << (int)readByte( pc+1 ) << "),C");
				LD8BitRegToAddrsOfIXd( regC,readByte( ++pc ) );
				break;
			case 0x72:
				//printf( "LD (IX+" << (int)readByte( pc+1 ) << "),D");
				LD8BitRegToAddrsOfIXd( regD,readByte( ++pc ) );
				break;
			case 0x73:
				//printf( "LD (IX+" << (int)readByte( pc+1 ) << "),E");
				LD8BitRegToAddrsOfIXd( regE,readByte( ++pc ) );
				break;
			case 0x74:
				//printf( "LD (IX+" << (int)readByte( pc+1 ) << "),H");
				LD8BitRegToAddrsOfIXd( regH,readByte( ++pc ) );
				break;
			case 0x75:
				//printf( "LD (IX+" << (int)readByte( pc+1 ) << "),L");
				LD8BitRegToAddrsOfIXd( regL,readByte( ++pc ) );
				break;
			case 0x77:
				//printf( "LD (IX+" << (int)readByte( pc+1 ) << "),A");
				LD8BitRegToAddrsOfIXd( regA,readByte( ++pc ) );
				break;
			case 0x7E:
				//printf( "LD A,(IX+" << (int)readByte( pc+1 ) << ")");
				LDAddrsOfIXdTo8BitReg(regA, readByte(++pc) );
				break;
			case 0xE1:
				//printf( "POP IX");
				POPIX();
				break;
			case 0xE3:
				//printf( "EX (SP),IX");
				EXIXToAddrsOfSP();
				break;
			case 0xE5:
				//printf( "PUSH IX");
				PUSHIX();
				break;
			case 0xF9:
				//printf( "LD SP,IX");
				LDIXRegToSPReg();
				break;*/
			default:
				break;
			}
			break;/*
		case 0xDE:
			break;
		case 0xDF:
			break;
		case 0xE0:
			break;*/
		case 0xE1:
			//printf( "POP HL");
			//POP( regH, regL );
			break;/*
		case 0xE2:
			break;*/
		case 0xE3:
			//printf( "EX (SP),HL");
			//EXHLToAddrsOfSP();
			break;/*
		case 0xE4:
			break;*/
		case 0xE5:
			//printf( "PUSH HL");
			//PUSH( regH, regL );
			break;
		break;/*
		case 0xE6:
			//printf("");
			break;
		case 0xE7:
			//printf("");
			break;
		case 0xE8:
			//printf("");
			break;
		case 0xE9:
			//printf( "JP (HL)");
		break;
		case 0xEA:
			//printf( "");
			break;*/
		case 0xEB:
			//printf( "EX DE, HL");
			//EXDEHL();
			break;/*
		case 0xEC:
			//printf("");
			break;*/
		case 0xED:
			switch( (int)readByte( ++pc ) ){/*
			case 0x43:
				//printf( "LD (" << std::hex << (int)readByte(pc+1) << (int)readByte(pc+2) << "),BC");
				LD16BitRegToAddrsOf16BitInt( readByte( ++pc ),readByte( ++pc ), regB, regC );				
				break;
			case 0x47:
				//printf( "LD I,A");
				LDAToI();
				break;
			case 0x4B:
				//printf( "LD BC(" << (int)readByte( pc+2 ) << (int)readByte( pc+1 ) << ")");
				LDAddrsOf16BitIntTo16BitReg( regB, regC, readByte( ++pc ), readByte( ++pc ) );
				break;
			case 0x4F:
				//printf( "LD R,A");
				LDAToR();
				break;
			case 0x52:
				//printf( "SBC HL,DE");
				SBC16BitRegToHL( regD, regE );
				break;
			case 0x53:
				//printf( "LD (" << (int)readByte(pc+1) << (int)readByte(pc+2) << "),DE");
				LD16BitRegToAddrsOf16BitInt( readByte( ++pc ),readByte( ++pc ), regD, regE );				
				break;
			case 0x56:
				//printf( "IM1" ); 
				IM1();
				break;
			case 0x57:
				//printf( "LD A,I");
				LDIToA();
				break;
			case 0x5F:
				//printf( "LD A,R");
				LDRToA();
				break;
			case 0xA0:
				//printf( "LDI");
				LDI();
				break;
			case 0xA1:
				//printf( "CPI");
				CPI();
				break;
			case 0xA8:
				//printf( "LLD");
				LDD();
				break;
			case 0xEDA9:
				//printf( "CPD");
				CPD();
				break;
			case 0xB0:
				//printf( "LDIR");
				LDIR();
				break;
			case 0xB1:
				//printf( "CPIR");
				CPIR();
				break;
			case 0xB8:
				//printf( "LDDR");
				LDDR();
				break;
			case 0xB9:
				//printf( "CPDR");
				CPDR();
				break;*/
			default:
				break;
			}
		break;/*
		case 0xEE:
			//printf("");
			break;
		case 0xEF:
			break;
		case 0xF0:
			break;*/
		case 0xF1:
			//printf( "POP AF");
			//POP( regA, regF );
			break;/*
		case 0xF2:
			break;*/
		case 0xF3:
			//printf("DI");
			//DI();
		break;/*
		case 0xF4:
			break;*/
		case 0xF5:
			//printf( "PUSH AF");
			//PUSH( regA, regF );
			break;/*
		case 0xF6:
			break;
		case 0xF7:
			//printf( "RST 30H");
		break;
		case 0xF8:
			break;*/
		case 0xF9:
			//printf( "LD SP,HL");
			//LDHLRegToSPReg();
			break;/*
		case 0xFA:
			break;*/
		case 0xFB:
			//printf( "EI" );
			//EI();
			break;/*
		case 0xFC:
			break;*/
		case 0xFD:
			switch( (int)readByte( ++pc ) ){/*
			case 0x21:
				//printf( "LD IY," << (int)readByte(pc+1) << (int)readByte(pc+2);
				LD16BitIntToIYReg( readByte(++pc), readByte(++pc) );				
				break;
			case 0x22:
				//printf( "LD (" << (int)readByte( pc+1 ) << (int)readByte(pc+2) << "),IX");
				LDIYRegToAddrsOf16BitInt( readByte( ++pc ), readByte( ++pc ) );	
				break;	
			case 0x2A:
				//printf( "LD IY(" << (int)readByte( pc+1 ) << (int)readByte( pc+2 ) << ")");
				LD16BitAddrsOf16BitIntToIY( readByte( ++pc ), readByte( ++pc ) );
				break;
			case 0x35:
				//printf( "DEC IY +%d", readByte( pc+1 ) );
				DECAddrsOfIYOffset( readByte( ++pc ) );
				break;
			case 0x36:
				//printf( "LD (IY+"<< (int)readByte(pc+1) << ")" << (int)readByte(pc+2);
				LD8BitIntToAddrsOfIYd( readByte(++pc), readByte(++pc) );
				break;
			case 0x46:
				//printf( "LD B,(IY+%d", (int)readByte( pc+1 ) );
				LDAddrsOfIYdTo8BitReg(regB, readByte(++pc) ); 
				break;
			case 0x4E:
				//printf( "LD C,(IY+" << (int)readByte( pc+1 ) << ")");
				LDAddrsOfIYdTo8BitReg(regC, readByte(++pc) );
				break;
			case 0x5E:
				//printf( "LD D,(IY+" << (int)readByte( pc+1 ) << ")");
				LDAddrsOfIYdTo8BitReg(regD, readByte(++pc) );
				break;
			case 0x56:
				//printf( "LD E,(IY+" << (int)readByte( pc+1 ) << ")");
				LDAddrsOfIYdTo8BitReg(regE, readByte(++pc) );
				break;
			case 0x66:
				//printf( "LD H,(IY+" << (int)readByte( pc+1 ) << ")");
				LDAddrsOfIYdTo8BitReg(regH, readByte(++pc) );
				break;
			case 0x6E:
				//printf( "LD L,(IY+" << (int)readByte( pc+1 ) << ")");
				LDAddrsOfIYdTo8BitReg(regL, readByte(++pc) );
				break;
			case 0x70:
				//printf( "LD (IY+" << (int)readByte( pc+1 ) << "),B");
				LD8BitRegToAddrsOfIYd( regB,readByte( ++pc ) );
				break;
			case 0x71:
				//printf( "LD (IY+" << (int)readByte( pc+1 ) << "),C");
				LD8BitRegToAddrsOfIYd( regC,readByte( ++pc ) );
				break;
			case 0x72:
				//printf( "LD (IY+" << (int)readByte( pc+1 ) << "),D");
				LD8BitRegToAddrsOfIYd( regD,readByte( ++pc ) );
				break;
			case 0x73:
				//printf( "LD (IY+" << (int)readByte( pc+1 ) << "),E");
				LD8BitRegToAddrsOfIYd( regE,readByte( ++pc ) );
				break;
			case 0x74:
				//printf( "LD (IY+" << (int)readByte( pc+1 ) << "),H");
				LD8BitRegToAddrsOfIYd( regH,readByte( ++pc ) );
				break;
			case 0x75:
				//printf( "LD (IY+" << (int)readByte( pc+1 ) << "),L");
				LD8BitRegToAddrsOfIYd( regL,readByte( ++pc ) );
				break;
			case 0x77:
				//printf( "LD (IY+" << (int)readByte( pc+1 ) << "),A");
				LD8BitRegToAddrsOfIYd( regL,readByte( ++pc ) );
				break;
			case 0x7E:
				//printf( "LD A,(IX+" << (int)readByte( pc+1 ) << ")");
				LDAddrsOfIYdTo8BitReg(regA, readByte(++pc) );
				break;
			case 0xE1:
				//printf( "POP IY");
				POPIY();
				break;
			case 0xE3:
				//printf( "EX (SP),IY");
				EXIYToAddrsOfSP();
				break;
			case 0xE5:
				//printf( "PUSH IY");
				PUSHIY();
				break;
			case 0xF9:
				//printf( "LD SP,IY");
				LDIYRegToSPReg();
				break;*/
			default:
				break;
			}
		break;/*
		case 0xFE:
			//printf( "CP %d");
		break;
		case 0xFF:
			//printf( "00");
			pc++;
		break;*/
		default:
			break;
		} 
		

		//printf( "\n");
}

void start( uint16_t addrs ){

	// Begin executing from 0x00
	// setPC( addrs );

	//While less that 16k rom
	while( pc < 0x12A2 ){			

		// Execute the opcode pointed to by program counter
		//execute( );
		pc++;

		//system("cls");
		/*
		printf("\nMain registers\tAlternate registers\n");
		printf("AF: %02x %02x", regA, regF );  
		printf("\tAF': %02x %02x\n",aregA, aregF );
		printf("BC: %02x %02x",regB, regC );  
		printf("\tBC': %02x %02x\n",aregB, aregC );
		printf("DE: %02x %02x",regD, regE );  
		printf("\tDE': %02x %02x\n", aregD, aregE );

		printf("HL: %02x %02x",regH, regL );  
		printf("\tHL': %02x %02x\n", aregH, aregL );

		printf("\nSP: %04x",sp );
	      	printf( "\nPC: %04x",pc );	

		printf("\nIX: %04x",indexIX);
		printf("\tIY: %04x", indexIY);

		printf("\nI: %02x", regI);
	        printf("\t\tR: %02x\n", regR);
	
		printf("\nEFF 1: %d", IFF2);
		printf("\tEFF 2: %d\n", IFF1);
		*/
	}
}

// Return the 16-bit value of two 8-bit combined registers
uint16_t byteToWord( uint8_t *byte1, uint8_t *byte2 ){
	uint16_t word = 0x00;
	
	word = (word | *byte1);

	word <<= 8;

	word = (word | *byte2);

	return word;
}

// Returns the LO byte of a word *could make into macro functions *
uint8_t getLOByte( uint16_t *word ){
	return (*word ^ 0xff00); // XOR the word with 1111111100000000 to zeros the HO byte
}

// Returns the HO byte of a word
uint8_t getHObyte( uint16_t *word ){
	return ((*word ^ 0xff) >> 8); // XOR the word with 1111111100000000 to zeros the HO byte then bit shift 8 places to the right
}

// Memory handling

// Read byte from memory given 16-bit address
uint8_t readByte( uint16_t addrs ){
	// Dereferecne into the memory location and return the value
	//return ( unsigned int )*memPtr->getByte( addrs );
	return 0;
}

// Write byte to memory given 16-bit address
void writeByte( uint16_t addrs, uint8_t val ){
	//setByte(addrs, val);
}


