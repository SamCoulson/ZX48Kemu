#include "../include/cpu.h"
#include <assert.h>
#include "stdio.h"

CPU::CPU(Memory* memPtr){
		
	// Pointer the instruction pointer at the beginning to a memory address
	this->memPtr = memPtr;

	// Reset all registers
	regA = 0x00;
	regF = 0x00;
	regB = 0x00;
	regC = 0x00;
	regD = 0x00;
	regE = 0x00;
	regH = 0x00;
	regL = 0x00;

	// 8-Bit Z80 alternative general purpose registers
	aregA = 0x00;
	aregF = 0x00; 
	aregB = 0x00;
	aregC = 0x00;
	aregD = 0x00;
	aregE = 0x00;
	aregH = 0x00;
	aregL = 0x00;

	// 8-Bit special registers - Interrupt vector
	regI = 0x00;
	regR = 0x00;

	// 16-Bit index registers
	indexIX = 0x0000;
	indexIY = 0x0000;

	// 16-Bit stack pointer and program counter 
	sp = 0x0000;

	// Set IFFI  **CHECK THIS BECAUSE CPU MUST START IN SOME KIND OF INTERRUPT MODE**
	IFF1 = false;
	IFF2 = false;
	
}

//  Set the PC to point to a location in memory
void CPU::setPC(uint16_t mem){
	pc = mem;
}

void CPU::execute( uint8_t* opcode ){

	std::cout << std::showbase << std::hex << pc << std::dec << std::noshowbase <<"\t";

	switch( *opcode ){
		/*case 0x00:
			std::cout << "NOP";
			pc++;
		break;*/
		case 0x01:
			LD16BitConstTo16BitReg(regB, regC, readByte(++pc), readByte(pc++) );
			std::cout << "LD BC,"<< std::hex << (int)regB << (int)regC;
			break;
		case 0x02:
			std::cout << "LD(BC),A";
			LDAToAddrsOfBC();
			break;/*
		case 0x03:
			std::cout << "JR NC,";
		break;
		case 0x04:
			std::cout << "INC B";
			INC(regB);
			break;
		case 0x05:
			break;*/
		case 0x06:
			std::cout << "LD B," << readByte(++pc);
			LD8BitIntTo8BitReg( regB, readByte(pc) );
			break;/*
		case 0x07:
			break;*/
		case 0x08:
			std::cout << "EX AF,AF'";
			EXAFAltAF();
			break;/*
		case 0x09:
			break;*/
		case 0x0A:
			std::cout << "LD A,(BC)";
			LDAddrsOfBCToA();
			break;/*
		case 0x0B:
			break;
		case 0x0C:
			break;
		case 0x0D:
			break;*/
		case 0x0E:
			std::cout << "LD C," << readByte(++pc);
			LD8BitIntTo8BitReg( regC, readByte(pc) );
			break;/*
		case 0x0F:
			break;
		case 0x10:
			break;*/
		case 0x11:
			LD16BitConstTo16BitReg(regD, regE, readByte(++pc), readByte(++pc) );
			std::cout << "LD DE,"<< std::hex << (int)regD << (int)regE;
			break;
		case 0x12:
			std::cout << "LD(DE),A";
			LDAToAddrsOfDE();
			break;/*
		case 0x13:
			break;
		case 0x14:
			break;
		case 0x15:
			break;*/
		case 0x16:
			std::cout << "LD D," << (int)readByte(++pc);
			LD8BitIntTo8BitReg( regD, readByte(pc) );
			break;/*
		case 0x17:
			break;
		case 0x18:
			std::cout << "JR 00%X";
			break;
		case 0x19:
			std::cout << "ADD HL,DE";
			ADD(regH, regL, regD, regE);
			break;*/
		case 0x1A:
			std::cout << "LD A,(DE)";
			LDAddrsOfDEToA();
			break;
		case 0x1B:/*
			break;
		case 0x1C:
			break;
		case 0x1D:
			break;*/
		case 0x1E:
			std::cout << "LD E," << readByte(++pc);
			LD8BitIntTo8BitReg( regE, readByte(pc) );
			break;/*
		case 0x1F:
			break;
		case 0x20:
			JRNZ( memPtr->getByte( ++pc ) );
			std::cout << "JR NZ," << (unsigned int)*memPtr->getByte( pc );
			break;*/
		case 0x21:
			LD16BitConstTo16BitReg(regH, regL, readByte(++pc), readByte(pc++) );
			std::cout << "LD HL,"<< std::hex << (int)regH << (int)regL;
			break;
		case 0x22:
			std::cout << "LD (" << std::hex << (int)readByte( pc+1 ) << (int)readByte( pc+2 ) << std::dec << "),HL";
			LDHLRegToAddrsOf16BitInt( readByte( ++pc ), readByte( ++pc ) );
							
			break;/*
		case 0x23:
			std::cout << "INC HL";
			INC(regH, regL);
			break;
		case 0x24:
			break;
		case 0x25:
			break;*/
		case 0x26:
			std::cout << "LD H," << readByte(++pc);
			LD8BitIntTo8BitReg( regH, readByte(pc) );
			break;/*
		case 0x27:
			break;
		case 0x28:
			std::cout << "JR Z," << (unsigned int)*memPtr->getByte( pc+2 );
			JRZ( memPtr->getByte( ++pc ) );
			break;
		case 0x29:
			break;*/
		case 0x2A:
			std::cout << "LD HL,(" << (int)readByte( pc+1 ) << (int)readByte( pc+2 ) << ")"; 
			LDAddrsOf16BitIntToHLReg( readByte( ++pc ), readByte( ++pc )  );
			break;/*
		case 0x2B:
			std::cout << "DEC HL";
			DEC(regH,regL);
		break;
		case 0x2C:
			break;
		case 0x2D:
			break;*/
		case 0x2E:
			std::cout << "LD L," << readByte(++pc);
			LD8BitIntTo8BitReg( regL, readByte(pc) );
			break;/*
		case 0x2F:
			break;
		case 0x30:
			std::cout << "JR NC," << (unsigned int)*memPtr->getByte( pc+1 );
			JRNC( memPtr->getByte( ++pc ) );
			break;
		case 0x31:
			LD(regS, regP, readByte(pc+=2), readByte(pc-1) );
			std::cout << "LD SP,"<< std::hex << (int)regH << (int)regL;
			break;*/
		case 0x32:
			std::cout << std::hex << "LD (" << readByte(pc+1) << readByte(pc+2) << "),A" << std::dec;
			LDAToAddrsOf16BitInt( readByte(++pc), readByte(++pc) );
			break;/*
		case 0x33:
			break;
		case 0x34:
			break;
		case 0x35:
			printf("");
			break;*/
		case 0x36:
			LD8BitIntToAddrsOfHL( readByte( ++pc ) );
			std::cout << "LD (HL),+" << readByte( pc-1 );
			break;/*
		case 0x37:
			std::cout << "SCF";
		break;
		case 0x38:
			std::cout << "JR C,%d";
		break;
		case 0x39:
			printf("");
			break;*/
		case 0x3A:
			std::cout << "LD A,(" << readByte( pc+1 ) << readByte( pc+2 ) << ")";
			LDAddrsOf16BitIntToA( regA, readByte( ++pc ), readByte( ++pc ) );
			break;/*
		case 0x3B:
			printf("");
			break;
		case 0x3C:
			printf("");
			break;
		case 0x3D:
			std::cout << "DEC A";
		break;*/
		case 0x3E:
			std::cout << "LD A," << readByte(++pc);
			LD8BitIntTo8BitReg( regA, readByte(pc) );
		break;/*
		case 0x3F:
			std::cout << "CCF";
		break;*/
		case 0x40:
			std::cout << "LD B,B";
			LD8BitRegTo8BitReg( regB, regB );
			break;
		case 0x41:
			std::cout << "LD B,C";
			LD8BitRegTo8BitReg( regB, regC );
			break;
		case 0x42:
			std::cout << "LD B,D";
			LD8BitRegTo8BitReg( regB, regD );
			break;
		case 0x43:
			std::cout << "LD B,E";
			LD8BitRegTo8BitReg( regB, regE );
			break;
		case 0x44:
			std::cout << "LD B,H";
			LD8BitRegTo8BitReg( regB, regH );
			break;
		case 0x45:
			std::cout << "LD B,L";
			LD8BitRegTo8BitReg( regB, regL );
			break;
		case 0x46:
			std::cout << "LD B,(HL)";
			LDAddrsOfHLTo8BitReg( regB );
			break;
		case 0x47:
			std::cout << "LD B,A";
			LD8BitRegTo8BitReg( regB, regA );
			break;
		case 0x48:
			std::cout << "LD C,B";
			LD8BitRegTo8BitReg( regB, regB );
			break;
		case 0x49:
			std::cout << "LD C,C";
			LD8BitRegTo8BitReg( regB, regC );
			break;
		case 0x4A:
			std::cout << "LD C,D";
			LD8BitRegTo8BitReg( regB, regD );
			break;
		case 0x4B:
			std::cout << "LD C,E";
			LD8BitRegTo8BitReg( regB, regE );
			break;
		case 0x4C:
			std::cout << "LD C,H";
			LD8BitRegTo8BitReg( regB, regH );
			break;
		case 0x4D:
			std::cout << "LD C,L";
			LD8BitRegTo8BitReg( regB, regL );
			break;
		case 0x4E:
			std::cout << "LD C,(HL)";
			LDAddrsOfHLTo8BitReg( regC );
			break;
		case 0x4F:
			std::cout << "LD C,A";
			LD8BitRegTo8BitReg( regB, regA );
			break;
		case 0x50:
			std::cout << "LD D,B";
			LD8BitRegTo8BitReg( regD, regB );
			break;
		case 0x51:
			std::cout << "LD D,C";
			LD8BitRegTo8BitReg( regD, regC );
			break;
		case 0x52:
			std::cout << "LD D,D";
			LD8BitRegTo8BitReg( regD, regD );
			break;
		case 0x53:
			std::cout << "LD D,E";
			LD8BitRegTo8BitReg( regD, regE );
			break;
		case 0x54:
			std::cout << "LD D,H";
			LD8BitRegTo8BitReg( regD, regH );
			break;
		case 0x55:
			std::cout << "LD D,L";
			LD8BitRegTo8BitReg( regD, regL );
			break;
		case 0x56:
			std::cout << "LD D,(HL)";
			LDAddrsOfHLTo8BitReg( regB );
			break;
		case 0x57:
			std::cout << "LD E,A";
			LD8BitRegTo8BitReg( regD, regA );
			break;
		case 0x58:
			std::cout << "LD E,B";
			LD8BitRegTo8BitReg(regE, regB);
			break;
		case 0x59:
			std::cout << "LD E,C";
			LD8BitRegTo8BitReg(regE, regC);
			break;
		case 0x5A:
			std::cout << "LD E,D";
			LD8BitRegTo8BitReg(regE, regD);
			break;
		case 0x5B:
			std::cout << "LD E,E";
			LD8BitRegTo8BitReg(regE, regE);
			break;
		case 0x5C:
			std::cout << "LD E,H";
			LD8BitRegTo8BitReg(regE, regH);
			break;
		case 0x5D:
			std::cout << "LD E,L";
			LD8BitRegTo8BitReg(regE, regL);
			break;
		case 0x5E:
			std::cout << "LD E,(HL)";
			LDAddrsOfHLTo8BitReg( regE );
			break;
		case 0x5F:
			std::cout << "LD D,A";
			LD8BitRegTo8BitReg(regD, regA);
			break;
		case 0x60:
			std::cout << "LD H,B";
			LD8BitRegTo8BitReg(regH, regB);
			break;
		case 0x61:
			std::cout << "LD H,C";
			LD8BitRegTo8BitReg(regH, regC);
			break;
		case 0x62:
			std::cout << "LD H,D";
			LD8BitRegTo8BitReg(regH, regD);
			break;
		case 0x63:
			std::cout << "LD H,E";
			LD8BitRegTo8BitReg(regH, regE);
			break;
		case 0x64:
			std::cout << "LD H,H";
			LD8BitRegTo8BitReg(regH, regH);
			break;
		case 0x65:
			std::cout << "LD H,L";
			LD8BitRegTo8BitReg(regH, regL);
			break;
		case 0x66:
			std::cout << "LD H,(HL)";
			LDAddrsOfHLTo8BitReg( regH );
			break;
		case 0x67:
			std::cout << "LD H,A";
			LD8BitRegTo8BitReg(regH, regA);
			break;
		case 0x68:
			std::cout << "LD L,B";
			LD8BitRegTo8BitReg(regL, regB);
			break;
		case 0x69:
			std::cout << "LD L,C";
			LD8BitRegTo8BitReg(regL, regC);
			break;
		case 0x6A:
			std::cout << "LD L,D";
			LD8BitRegTo8BitReg(regL, regD);
			break;
		case 0x6B:
			std::cout << "LD L,E";
			LD8BitRegTo8BitReg(regL, regE);
			break;
		case 0x6C:
			std::cout << "LD L,H";
			LD8BitRegTo8BitReg(regL, regH);
			break;
		case 0x6D:
			std::cout << "LD L,L";
			LD8BitRegTo8BitReg(regL, regL);
			break;
		case 0x6E:
			std::cout << "LD L,(HL)";
			LDAddrsOfHLTo8BitReg( regL );
			break;
		case 0x6F:
			std::cout << "LD L,A";
			LD8BitRegTo8BitReg(regL, regA);
			break;
		case 0x70:
			std::cout << "(HL),B";
			LD8BitRegToAddrsOfHL(regB);
			break;
		case 0x71:
			std::cout << "(HL),C";
			LD8BitRegToAddrsOfHL(regC);
			break;
		case 0x72:
			std::cout << "(HL),D";
			LD8BitRegToAddrsOfHL(regD);
			break;
		case 0x73:
			std::cout << "(HL),E";
			LD8BitRegToAddrsOfHL(regE);
			break;
		case 0x74:
			std::cout << "(HL),H";
			LD8BitRegToAddrsOfHL(regH);
			break;
		case 0x75:
			std::cout << "(HL),L";
			LD8BitRegToAddrsOfHL(regL);
		break;/*
		case 0x76:
			cout << "HALT";
		break;*/
		case 0x77:
			std::cout << "(HL),A";
			LD8BitRegToAddrsOfHL(regA);
		break;
		case 0x78:
			std::cout << "LD A,B";
			LD8BitRegTo8BitReg(regA, regB);
		break;
		case 0x79:
			std::cout << "LD A,C";
			LD8BitRegTo8BitReg(regA, regC);
		break;
		case 0x7A:
			std::cout << "LD A,D";
			LD8BitRegTo8BitReg(regA, regD);
		break;
		case 0x7B:
			std::cout << "LD A,E";
			LD8BitRegTo8BitReg(regA, regE);
		break;
		case 0x7C:
			std::cout << "LD A,H";
			LD8BitRegTo8BitReg(regA, regH);
		break;
		case 0x7D:
			std::cout << "LD A,L";
			LD8BitRegTo8BitReg(regA, regL);
			break;
		case 0x7E:
			std::cout << "LD A,(HL)";
			LDAddrsOfHLTo8BitReg( regA );
			break;
		case 0x7F:
			std::cout << "LD A,A";
			LD8BitRegTo8BitReg(regA, regA);
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
			break;
		case 0xA7:
			std::cout << "AND A";
			AND(regA);
		break;
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
			break;
		case 0xAF:
			std::cout << "XOR A";
				XOR(regA);
			break;
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
			std::cout << "OR L";
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
			break;
		case 0xBC:
			std::cout << "CP H";
			CP(regH);
		break;
		case 0xBD:
			break;
		case 0xBE:
			break;
		case 0xBF:
			std::cout << "CP A";
		break;
		case 0xC0:
			std::cout << "RET NZ";
		break;*/
		case 0xC1:
			std::cout << "POP BC";
			POP( regB, regC );
			break;/*
		case 0xC2:
			break;
		case 0xC3:
			std::cout << "JP " << std::hex << (unsigned int)*memPtr->getByte( pc+2 ) << (unsigned int)*memPtr->getByte( pc+1 );
			JP(memPtr->getByte( pc+=1 ), memPtr->getByte( pc+=1 ) );
		break;
		case 0xC4:
			break;*/
		case 0xC5:
			std::cout << "PUSH BC";
			PUSH( regB, regC );
			break;
		break;/*
		case 0xC6:
			break;
		case 0xC7:
			break;
		case 0xC8:
			std::cout << "RET Z";
		break;
		case 0xC9:
			std::cout << "RET";
		break;
		case 0xCA:
			break;
		case 0xCB:
			break;
		case 0xCC:
			break;
		case 0xCD:
			std::cout << "CALL %X%X";
		break;
		case 0xCE:
			break;
		case 0xCF:
			break;
		case 0xD0:
			std::cout << "RET NC";
		break;*/
		case 0xD1:
			std::cout << "POP DE";
			POP( regD, regE );
			break;/*
		case 0xD2:
			break;
		case 0xD3:
			OUTA(*memPtr->getByte( pc++ ), &regA);
			std::cout << "OUT (" << (unsigned int)*memPtr->getByte( pc-1 ) << "),A";
			break;
		case 0xD4:
			std::cout << "CALL NC,%X%X";
			break;*/
		case 0xD5:
			std::cout << "PUSH DE";
			PUSH( regD, regE );
			break;/*
		case 0xD6:
			break;
		case 0xD7:
			break;
		case 0xD8:
			std::cout << "RET C";
			break;*/
		case 0xD9:
			std::cout << "EXX";
			EXX();
			break;/*
		case 0xDA:
			break;
		case 0xDB:
			break;
		case 0xDC:
			break;*/
		case 0xDD:
			switch( ++pc ){
			case 0x21:
				std::cout << "LD IX," << (int)readByte( pc+1 ) << (int)readByte(pc+2);
				LD16BitIntToIXReg( readByte(++pc), readByte(++pc) );
				break;
			case 0x22:
				std::cout << "LD (" << (int)readByte( pc+1 ) << (int)readByte(pc+2) << "),IX";
				LDIXRegToAddrsOf16BitInt( readByte( ++pc ), readByte( ++pc ) );	
				break;
			case 0x2A:
				std::cout << "LD IX(" << (int)readByte( pc+1 ) << (int)readByte( pc+2 ) << ")";
				LD16BitAddrsOf16BitIntToIX( readByte( ++pc ), readByte( ++pc ) );
				break;
			case 0x36:
				std::cout << "LD (IX+"<< (int)readByte(pc+1) << ")" << (int)readByte(pc+2);
				LD8BitIntToAddrsOfIXd( readByte(++pc), readByte(++pc) );
				break;
			case 0x46:
				std::cout << "LD B,(IX+" << (int)readByte( pc+1 ) << ")";
				LDAddrsOfIXdTo8BitReg(regB, readByte(++pc) ); 
				break;
			case 0x4E:
				std::cout << "LD C,(IX+" << (int)readByte( pc+1 ) << ")";
				LDAddrsOfIXdTo8BitReg(regC, readByte(++pc) );
				break;
			case 0x5E:
				std::cout << "LD D,(IX+" << (int)readByte( pc+1 ) << ")";
				LDAddrsOfIXdTo8BitReg(regD, readByte(++pc) );
				break;
			case 0x56:
				std::cout << "LD E,(IX+" << (int)readByte( pc+1 ) << ")";
				LDAddrsOfIXdTo8BitReg(regE, readByte(++pc) );
				break;
			case 0x66:
				std::cout << "LD H,(IX+" << (int)readByte( pc+1 ) << ")";
				LDAddrsOfIXdTo8BitReg(regH, readByte(++pc) );
				break;
			case 0x6E:
				std::cout << "LD L,(IX+" << (int)readByte( pc+1 ) << ")";
				LDAddrsOfIXdTo8BitReg( regL, readByte(++pc) );
				break;
			case 0x70:
				std::cout << "LD (IX+" << (int)readByte( pc+1 ) << "),B";
				LD8BitRegToAddrsOfIXd( regB,readByte( ++pc ) );
				break;
			case 0x71:
				std::cout << "LD (IX+" << (int)readByte( pc+1 ) << "),C";
				LD8BitRegToAddrsOfIXd( regC,readByte( ++pc ) );
				break;
			case 0x72:
				std::cout << "LD (IX+" << (int)readByte( pc+1 ) << "),D";
				LD8BitRegToAddrsOfIXd( regD,readByte( ++pc ) );
				break;
			case 0x73:
				std::cout << "LD (IX+" << (int)readByte( pc+1 ) << "),E";
				LD8BitRegToAddrsOfIXd( regE,readByte( ++pc ) );
				break;
			case 0x74:
				std::cout << "LD (IX+" << (int)readByte( pc+1 ) << "),H";
				LD8BitRegToAddrsOfIXd( regH,readByte( ++pc ) );
				break;
			case 0x75:
				std::cout << "LD (IX+" << (int)readByte( pc+1 ) << "),L";
				LD8BitRegToAddrsOfIXd( regL,readByte( ++pc ) );
				break;
			case 0x77:
				std::cout << "LD (IX+" << (int)readByte( pc+1 ) << "),A";
				LD8BitRegToAddrsOfIXd( regA,readByte( ++pc ) );
				break;
			case 0x7E:
				std::cout << "LD A,(IX+" << (int)readByte( pc+1 ) << ")";
				LDAddrsOfIXdTo8BitReg(regA, readByte(++pc) );
				break;
			case 0xE1:
				std::cout << "POP IX";
				POPIX();
				break;
			case 0xE3:
				std::cout << "EX (SP),IX";
				EXIXToAddrsOfSP();
				break;
			case 0xE5:
				std::cout << "PUSH IX";
				PUSHIX();
				break;
			case 0xF9:
				std::cout << "LD SP,IX";
				LDIXRegToSPReg();
				break;
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
			std::cout << "POP HL";
			POP( regH, regL );
			break;/*
		case 0xE2:
			break;*/
		case 0xE3:
			std::cout << "EX (SP),HL";
			EXHLToAddrsOfSP();
			break;/*
		case 0xE4:
			break;*/
		case 0xE5:
			std::cout << "PUSH HL";
			PUSH( regH, regL );
			break;
		break;/*
		case 0xE6:
			printf("");
			break;
		case 0xE7:
			printf("");
			break;
		case 0xE8:
			printf("");
			break;
		case 0xE9:
			std::cout << "JP (HL)";
		break;
		case 0xEA:
			std::cout << "";
			break;*/
		case 0xEB:
			std::cout << "EX DE, HL";
			EXDEHL();
			break;/*
		case 0xEC:
			printf("");
			break;*/
		case 0xED:
			switch( ++pc ){
			case 0x43:
				std::cout << "LD (" << readByte(pc+1) << readByte(pc+2) << "),BC";
				LD16BitRegToAddrsOf16BitInt( readByte( ++pc ),readByte( ++pc ), regB, regC );				
				break;
			case 0x47:
				std::cout << "LD I,A";
				LDAToI();
				break;
			case 0x4B:
				std::cout << "LD BC(" << readByte( pc+2 ) << readByte( pc+1 ) << ")";
				LDAddrsOf16BitIntTo16BitReg( regB, regC, readByte( ++pc ), readByte( ++pc ) );
				break;
			case 0x4F:
				std::cout << "LD R,A";
				LDAToR();
				break;/*
			case 0x52:
				std::cout << "SBC HL,DE";
				SBC(regH,regL, regD, regE);
				break;*/
			case 0x53:
				std::cout << "LD (" << readByte(pc+1) << readByte(pc+2) << "),DE";
				LD16BitRegToAddrsOf16BitInt( readByte( ++pc ),readByte( ++pc ), regD, regE );				
				break;/*
			case 0x56:
				std::cout << "IM1 (not yet implemented)"; 
				// Need to work out how the most is determined by the code, i.e. do we set a flag to set the mode??
				pc++;
				break;*/
			case 0x57:
				std::cout << "LD A,I";
				LDIToA();
				break;
			case 0x5F:
				std::cout << "LD A,R";
				LDRToA();
				break;
			case 0xA0:
				std::cout << "LDI";
				LDI();
				break;
			case 0xA1:
				std::cout << "CPI";
				CPI();
				break;
			case 0xA8:
				std::cout << "LLD";
				LDD();
				break;
			case 0xEDA9:
				std::cout << "CPD";
				CPD();
				break;
			case 0xB0:
				std::cout << "LDIR";
				LDIR();
				break;
			case 0xB1:
				std::cout << "CPIR";
				CPIR();
				break;
			case 0xB8:
				std::cout << "LDDR";
				LDDR();
				break;
			case 0xB9:
				std::cout << "CPDR";
				CPDR();
				break;
			default:
				break;
			}
		break;/*
		case 0xEE:
			printf("");
			break;
		case 0xEF:
			break;
		case 0xF0:
			break;*/
		case 0xF1:
			std::cout << "POP AF";
			POP( regA, regF );
			break;/*
		case 0xF2:
			break;
		case 0xF3:
			std::cout <<"DI (Not implement yet!)";
			pc++;
		break;
		case 0xF4:
			break;*/
		case 0xF5:
			std::cout << "PUSH AF";
			PUSH( regA, regF );
			break;/*
		case 0xF6:
			break;
		case 0xF7:
			std::cout << "RST 30H";
		break;
		case 0xF8:
			break;*/
		case 0xF9:
			std::cout << "LD SP,HL";
			LDHLRegToSPReg();
			break;/*
		case 0xFA:
			break;
		case 0xFB:
			std::cout << "EI (not implemented yet)";
			pc++;
			break;
		case 0xFC:
			break;*/
		case 0xFD:
			switch( ++pc ){
			case 0x21:
				std::cout << "LD IY," << (int)readByte(pc+1) << (int)readByte(pc+2);
				LD16BitIntToIYReg( readByte(++pc), readByte(++pc) );				
				break;
			case 0x22:
				std::cout << "LD (" << (int)readByte( pc+1 ) << (int)readByte(pc+2) << "),IX";
				LDIYRegToAddrsOf16BitInt( readByte( ++pc ), readByte( ++pc ) );	
				break;	
			case 0x2A:
				std::cout << "LD IY(" << (int)readByte( pc+1 ) << (int)readByte( pc+2 ) << ")";
				LD16BitAddrsOf16BitIntToIY( readByte( ++pc ), readByte( ++pc ) );
				break;/*
			case 0x35:
				std::cout << "DEC IY+" << (unsigned int)*memPtr->getByte( pc+1 );
				break;*/
			case 0x36:
				std::cout << "LD (IY+"<< (int)readByte(pc+1) << ")" << (int)readByte(pc+2);
				LD8BitIntToAddrsOfIYd( readByte(++pc), readByte(++pc) );
				break;
			case 0x46:
				std::cout << "LD B,(IY+" << (int)readByte( pc+1 ) << ")";
				LDAddrsOfIYdTo8BitReg(regB, readByte(++pc) ); 
				break;
			case 0x4E:
				std::cout << "LD C,(IY+" << (int)readByte( pc+1 ) << ")";
				LDAddrsOfIYdTo8BitReg(regC, readByte(++pc) );
				break;
			case 0x5E:
				std::cout << "LD D,(IY+" << (int)readByte( pc+1 ) << ")";
				LDAddrsOfIYdTo8BitReg(regD, readByte(++pc) );
				break;
			case 0x56:
				std::cout << "LD E,(IY+" << (int)readByte( pc+1 ) << ")";
				LDAddrsOfIYdTo8BitReg(regE, readByte(++pc) );
				break;
			case 0x66:
				std::cout << "LD H,(IY+" << (int)readByte( pc+1 ) << ")";
				LDAddrsOfIYdTo8BitReg(regH, readByte(++pc) );
				break;
			case 0x6E:
				std::cout << "LD L,(IY+" << (int)readByte( pc+1 ) << ")";
				LDAddrsOfIYdTo8BitReg(regL, readByte(++pc) );
				break;
			case 0x70:
				std::cout << "LD (IY+" << (int)readByte( pc+1 ) << "),B";
				LD8BitRegToAddrsOfIYd( regB,readByte( ++pc ) );
				break;
			case 0x71:
				std::cout << "LD (IY+" << (int)readByte( pc+1 ) << "),C";
				LD8BitRegToAddrsOfIYd( regC,readByte( ++pc ) );
				break;
			case 0x72:
				std::cout << "LD (IY+" << (int)readByte( pc+1 ) << "),D";
				LD8BitRegToAddrsOfIYd( regD,readByte( ++pc ) );
				break;
			case 0x73:
				std::cout << "LD (IY+" << (int)readByte( pc+1 ) << "),E";
				LD8BitRegToAddrsOfIYd( regE,readByte( ++pc ) );
				break;
			case 0x74:
				std::cout << "LD (IY+" << (int)readByte( pc+1 ) << "),H";
				LD8BitRegToAddrsOfIYd( regH,readByte( ++pc ) );
				break;
			case 0x75:
				std::cout << "LD (IY+" << (int)readByte( pc+1 ) << "),L";
				LD8BitRegToAddrsOfIYd( regL,readByte( ++pc ) );
				break;
			case 0x77:
				std::cout << "LD (IY+" << (int)readByte( pc+1 ) << "),A";
				LD8BitRegToAddrsOfIYd( regL,readByte( ++pc ) );
				break;
			case 0x7E:
				std::cout << "LD A,(IX+" << (int)readByte( pc+1 ) << ")";
				LDAddrsOfIYdTo8BitReg(regA, readByte(++pc) );
				break;
			case 0xE1:
				std::cout << "POP IY";
				POPIY();
				break;
			case 0xE3:
				std::cout << "EX (SP),IY";
				EXIYToAddrsOfSP();
				break;
			case 0xE5:
				std::cout << "PUSH IY";
				PUSHIY();
				break;
			case 0xF9:
				std::cout << "LD SP,IY";
				LDIYRegToSPReg();
				break;
			default:
				break;
			}
		break;/*
		case 0xFE:
			std::cout << "CP %d";
		break;
		case 0xFF:
			std::cout << "00";
			pc++;
		break;*/
		default:
			break;
		} 
		pc++;

		printf("\n");
}


// Implements the bitwise Exclusive OR operator, commonly used to zero a register 
void CPU::XOR( uint8_t &val ){
	val = val ^ val; 
	pc++;
}

// Output the contents of A to port n
void CPU::OUTA( uint8_t ioAddrs, uint8_t* val){
	pc++;
}

void CPU::JP( uint8_t *byte1, uint8_t *byte2 ){

	uint16_t word = 0x00;
	
	word = (word | *byte1);

	word <<= 8;

	word = (word | *byte2);

	setPC( word );
}

// Decrement the 16-bit ADDRESS pointer by 1
void CPU::DEC(uint8_t &HOreg, uint8_t &LOreg){

	// Join the 8-bit registers together to determine the 16-bit address
	uint16_t addrs = byteToWord(&HOreg, &LOreg);

	// Decrement the address by 1
	addrs--;

	// Get LObyte and copy to LOreg
	LOreg = getLOByte(&addrs);
	HOreg = getHOByte(&addrs);

	pc++;
}

void CPU::start(){
	
	// Begin executing from 0x00
	setPC(0x00);

	for(int i = 0; i < 125; i++){
		
		// Execute the opcode pointed to by program counter
		execute( memPtr->getByte( pc ) );

	}
}

// Return the 16-bit value of two 8-bit combined registers
uint16_t CPU::byteToWord(uint8_t *byte1, uint8_t *byte2){

	uint16_t word = 0x00;
	
	word = (word | *byte1);

	word <<= 8;

	word = (word | *byte2);

	return word;
}

// Returns the LO byte of a word *could make into macro functions *
uint8_t CPU::getLOByte(uint16_t *word){
	return (*word ^ 0xff00); // XOR the word with 1111111100000000 to zeros the HO byte
}

// Returns the HO byte of a word
uint8_t CPU::getHOByte(uint16_t *word){
	return ((*word ^ 0xff) >> 8); // XOR the word with 1111111100000000 to zeros the HO byte then bit shift 8 places to the right
}

// Compare register
void CPU::CP(uint8_t &reg){
	// Subtract register from register A, the value sets the flags in the Flags register F
	regF = regA - reg;
	pc++;
}

// Jump relative - Jump by val if Z in flags zero
void CPU::JRZ( uint8_t *val){
	// Logically AND register with 0x40 (01000000)
	if(!(regF & 0x40)){
		pc += *val;
	}
	pc++; // Always shift one more even when given a value it means jump x amount of bytes from the next address
}

// Jump relative - Jump by val if Z in flags is non-zero
void CPU::JRNZ( uint8_t *val){
	// Logically AND register with 0x40 (01000000)
	if(regF & 0x40){
		pc += *val;
	}
	pc++; // Always shift one more even when given a value it means jump x amount of bytes from the next address
}

void CPU::JRNC( uint8_t *val ){
	// Logically AND register with 0x01 (0000001)
	if(regF & 0x01){
		pc += *val;
	}
	pc++;
}

// Logically AND the register with itself
void CPU::AND(uint8_t &reg){
	reg &= reg;
	pc++;
}

// Subtract w/ carry flag the second 16-bit register value from the first 16-bit register value, store in the first 
void CPU::SBC(uint8_t &reg1, uint8_t &reg2, uint8_t &reg3, uint8_t &reg4){
	uint16_t result = ( byteToWord(&reg1, &reg2) - byteToWord(&reg3, &reg4) ) - (regF & 0x01);
	reg1 = getHOByte(&result);
	reg2 = getLOByte(&result);
	pc++;
}

// ADD the first 16-bit register to the second and store in the first
void CPU::ADD(uint8_t &reg1, uint8_t &reg2, uint8_t &reg3, uint8_t &reg4){
	uint16_t result = ( byteToWord(&reg1, &reg2) + byteToWord(&reg3, &reg4) );
	reg1 = getHOByte(&result);
	reg2 = getLOByte(&result);
	pc++;
}

// INC 16-bit
void CPU::INC(uint8_t &reg1, uint8_t &reg2){
	uint16_t val = byteToWord(&reg1, &reg2);
	val++;
	reg1 = getHOByte(&val);
	reg2 = getLOByte(&val);
	pc++;
}

// INC 8-bit
void CPU::INC(uint8_t &reg){
	reg++;
	pc++;
}

// Memory handling

// Read byte from memory given 16-bit address
uint8_t CPU::readByte( uint16_t addrs ){
	// Dereferecne into the memory location and return the value
	return ( unsigned int )*memPtr->getByte( addrs );
}

// Write byte to memory given 16-bit address
void CPU::writeByte( uint16_t addrs, uint8_t val ){
	memPtr->setByte(addrs, val);
}

// Set the given bit in a byte to either 1 or 0
void CPU::setBitInByte(unsigned char &byte, uint8_t pos, uint8_t val){
	
	if( (pos < 1 ) && (pos > 8) ){
		std::cout << "invalid bit range passed to setBitInByte";
		return;
	}

	if( val < 0 || val > 1 ){
		std::cout << "invalid bit value passed to setBitInByte";
		return;
	}

	if(val == 0)
		byte &= ~( 1 << (pos-1) ); // Set to OFF, 00000001 shifted to the position of pos, AND i.e. both need to be true to set to 1, 
									// then inverse the result with ~
	else
		byte |= ( 1 << (pos-1) ); // Set to ON, 00000001 shifted to the position of pos, OR i.e. either 0 or 1 will set to 1

	// pos is minus 1 because bytes work like a 0 based array
}

// Find the status of a specific bit **TEST THIS**
bool CPU::IsBitSet( uint8_t &byte, uint8_t pos ){

	if( (pos < 1 ) && (pos > 8) ){
		std::cout << "invalid bit range passed to setBitInByte";
		return false;
	}

	if( byte & ( 1 << ( pos-1) ) ){
		return true;
	}else{
		return false;
	}
}
