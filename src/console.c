#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "../include/console.h"
#include "../include/cpu.h"
#include "../include/util_bit_operations.h"

extern Registers *reg;

void console(){
	static uint8_t stepMode = 1;

	static int key = 0;
	static int mode = 0;
	
	//system("cls");
	
	if( *reg->pc == 0x12A9 )
		stepMode = 1;

	if( mode == 0 ){
		printf("\nMain registers\tAlternate registers\n");
		printf("AF: %02x %02x", *reg->a, *reg->f );  
		printf("\tAF': %02x %02x\n",*reg->alta, *reg->altf );
		printf("BC: %02x %02x", *reg->b, *reg->c );  
		printf("\tBC': %02x %02x\n", *reg->altb, *reg->altc );
		printf("DE: %02x %02x", *reg->d, *reg->e );  
		printf("\tDE': %02x %02x\n", *reg->altd, *reg->alte );

		printf("HL: %02x %02x", *reg->h, *reg->l );  
		printf("\tHL': %02x %02x\n", *reg->alth, *reg->altl );

		printf("\nSP: %04x", *reg->sp );
		printf( "\nPC: %04x\n", *reg->pc );	

		printf("\nIX: %04x", *reg->ix );
		printf("\tIY: %04x", *reg->iy );

		printf("\nI: %02x", *reg->i );
		printf("\t\tR: %02x\n", *reg->r );

		printf("\nEFF 1: %d", *reg->iff2 );
		printf("\tEFF 2: %d\n", *reg->iff1 );

		printf("\n*** Flags ***\n");
		printf("S:%d Z:%d -:%d H:%d -:%d P/V:%d N:%d C:%d",
				getBit( reg->f, 7 ),
				getBit( reg->f, 6 ),
				getBit( reg->f, 5 ),
				getBit( reg->f, 4 ),
				getBit( reg->f, 3 ),
				getBit( reg->f, 2 ),
				getBit( reg->f, 1 ),
				getBit( reg->f, 0 ) );

		printf( "\n\n*** Stack ***\n" );

		for( int i = 0xFF56; i > 0xFF43; i-- ){
			printf("\n%X %X", i, totalMem[i] );
			if( *reg->sp == i ){
				printf( "\t<" );
			}
			
		}
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" );
	}

	// Prints the channels 
	if( mode == 1 ){
		for( int i = 0x5CB6; i < 0x5CD7; i+=2 ){
			printf("\n%X %02X %02X", i, totalMem[i], totalMem[i+1] );
		}
	}

	// Print content of video memory
	if( mode == 3 ){
		// Row counter
		uint8_t column = 0;

		// Read the video memory
		for( int i = 0x4000; i < 0x57FF; i++ ){
			printf( "%02X", totalMem[i] );
			column++;
			if( i == 0x4800 ){
				printf("\nSecond third\n");
			}else if( i == 0x5000 ){
				printf("\nThird third\n");
			}

			if( column == 32 ){
				printf("\n");
				column = 0;
			}
		}	
	}


	if( mode == 2 ){
		for(int i = 0x5C00; i < 0x5CB5; i++ ){
			switch( i ){
				case 0x5C00 :
					printf( "KSTATE\t:\n" );
					for( int j = 0; j < 8; j++ ){
						printf( "\t%X %d: %02X\n",i ,j ,totalMem[i] );
						i++;
					}
					break;
				case 0x5C10 :
					printf( "STRMS\t:\n" );
					for( int j = 0; j < 38; j++ ){
						printf( "\t%X : %02X\n",i ,totalMem[i] );
						i++;
					}
					break;	
				case 0x5C3C :
					printf( "TVFLAGS\t: %02X\n", totalMem[i] );
					break;
				case 0x5C3D :
					printf( "ERR-SP\t: %02X %02X\n", totalMem[i], totalMem[i++] );
					break;
				case 0x5C4F :
					printf( "CHANS\t: %02X %02X\n", totalMem[i], totalMem[i++] );
					break;
				case 0x5C51 :
					printf( "CURCHL\t: %02X %02X\n", totalMem[i], totalMem[i++] );
					break;
				case 0x5C53 :
					printf( "PROG\t: %02X %02X\n", totalMem[i], totalMem[i++] );
					break;
				case 0x5C57:
					printf( "DATADD\t: %02X %02X\n", totalMem[i], totalMem[i++] );
					break;
				case 0x5C6A :
					printf( "FLAGS2\t: %02X\n", totalMem[i] );
					break;
				case 0x5C6B :
					printf( "DF-SZ\t: %02X\n", totalMem[i] );
					break;
				case 0x5C7F :
					printf( "P-POSN\t: %02X\n", totalMem[i] );	
					break;	
				case 0x5CB2 :
					printf( "RAM-TOP\t: %02X %02X\n", totalMem[i], totalMem[i++] );
					break;
				case 0x5CB4 :
					printf( "P-RAMT\t: %02X %02X\n", totalMem[i], totalMem[i++] );
					break;	
				default:
				break;
			}
		}
	}


	if( kbhit() || stepMode ){	
		key = getch();
		
		// printf("key press = %d\n", key);

		if( key == 113 ){
			exit(1); // key q
		}else if( key == 115 ){
			mode = 0; // key s
		}else if( key == 108 ) {
			mode = 4; // l key
		}else if( key == 109 ){
			mode = 1; // key m
		}else if( key == 114 ){
			stepMode = stepMode ? 0 : 1; // r
		}else if( key == 118 ){
			mode = 2;
		}else if( key == 107 ){
			mode = 3; // k key
		}		
	}
}


