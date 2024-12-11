#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "../include/console.h"
#include "../include/cpu.h"
#include "../include/memory.h"
#include "../include/util_bit_operations.h"

void console(){

	if(WindowShouldClose()){
		exit(1);
	}


	BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawText("Registers", 400, 10, 10, BLACK);
		DrawText(TextFormat("AF: %02X %02X", *reg->a, *reg->f), 400, 30, 10, BLACK);
		DrawText(TextFormat("AF': %02X %02X", *reg->alta, *reg->altf), 475, 30, 10, BLACK);
		DrawText(TextFormat("BC: %02X %02X", *reg->b, *reg->c), 400, 40, 10, BLACK);
		DrawText(TextFormat("BC': %02X %02X", *reg->altb, *reg->altc), 475, 40, 10, BLACK);
		DrawText(TextFormat("DE: %02X %02X", *reg->d, *reg->e), 400, 50, 10, BLACK);
		DrawText(TextFormat("DE': %02X %02X", *reg->altd, *reg->alte), 475, 50, 10, BLACK);

		DrawText(TextFormat("HL: %02X %02X", *reg->h, *reg->l), 400, 70, 10, BLACK);
		DrawText(TextFormat("HL': %02X %02X", *reg->alth, *reg->altl), 475, 70, 10, BLACK);

		DrawText(TextFormat("SP: %04X", *reg->sp), 400, 90, 10, BLACK);
		DrawText(TextFormat("PC: %04X", *reg->pc), 475, 90, 10, BLACK);

		DrawText(TextFormat("IX: %04X", *reg->ix), 400, 110, 10, BLACK);
		DrawText(TextFormat("IY: %04X", *reg->ix), 475, 110, 10, BLACK);

		DrawText(TextFormat("I: %02X", *reg->i), 400, 130, 10, BLACK);
		DrawText(TextFormat("R: %02X", *reg->r), 475, 130, 10, BLACK);

		DrawText(TextFormat("EFF 1: %d", *reg->iff1), 400, 150, 10, BLACK);
		DrawText(TextFormat("EFF 2: %d", *reg->iff2), 475, 150, 10, BLACK);

		DrawText("Flags", 400, 170, 10, BLACK);
		DrawText(TextFormat("S:%d Z:%d -:%d H:%d -:%d P/V:%d N:%d C:%d",
				getBit( reg->f, 7 ),
				getBit( reg->f, 6 ),
				getBit( reg->f, 5 ),
				getBit( reg->f, 4 ),
				getBit( reg->f, 3 ),
				getBit( reg->f, 2 ),
				getBit( reg->f, 1 ),
				getBit( reg->f, 0 ) ), 400, 190, 10, BLACK);

		DrawText("Stack",575, 10, 10, BLACK);

		int k = 30;
		for( int i = 0xFF56; i > 0xFF35; i-- ){
			DrawText(TextFormat("%X %X", i, totalMem[i]), 575, k, 10, BLACK);
			if( *reg->sp == i ){
				printf( "\t<" );
			}
			k+=10;
		}
	EndDrawing();

//	if( *reg->pc == 0x18E1 )
//		stepMode = 1;
//
//
//	// Prints the channels
//	if( mode == 1 ){
//		for( int i = 0x5CB6; i < 0x5CD7; i+=2 ){
//			printf("\n%X %02X %02X", i, totalMem[i], totalMem[i+1] );
//		}
//	}
//
//	// Print content of video memory
//	if( mode == 3 ){
//		// Row counter
//		uint8_t column = 0;
//
//		// Read the video memory
//		for( int i = 0x4000; i < 0x57FF; i++ ){
//			printf( "%02X", totalMem[i] );
//			column++;
//			if( i == 0x4800 ){
//				printf("\nSecond third\n");
//			}else if( i == 0x5000 ){
//				printf("\nThird third\n");
//			}
//
//			if( column == 32 ){
//				printf("\n");
//				column = 0;
//			}
//		}
//	}
//
//
//	if( mode == 2 ){
//		for(int i = 0x5C00; i < 0x5CB5; i++ ){
//			switch( i ){
//				case 0x5C00 :
//					printf( "KSTATE\t:\n" );
//					for( int j = 0; j < 8; j++ ){
//						printf( "\t%X %d: %02X\n",i ,j ,totalMem[i] );
//						i++;
//					}
//					--i;  // Step back one to compensate for last i++, this stops last-k
//					// being shown if it is omitted.
//					break;
//				case 0x5C08 :
//					printf( "LAST-K\t: %02X\n",totalMem[i]);
//					break;
//				case 0x5C09 :
//					printf( "REPDEL\t: %02X\n",totalMem[i]);
//					break;
//				case 0x5C0A :
//					printf( "REPPER\t: %02X\n",totalMem[i]);
//					break;
//				case 0x5C0D :
//					printf( "KDATA\t: %02X\n",totalMem[i]);
//					break;
//				case 0x5C10 :
//					printf( "STRMS\t:\n" );
//					for( int j = 0; j < 38; j++ ){
//						printf( "\t%X : %02X\n",i ,totalMem[i] );
//						i++;
//					}
//					break;
//				case 0x5C3B :
//					printf( "FLAGS\t: %02X\n", totalMem[i] );
//					break;
//				case 0x5C3C :
//					printf( "TVFLAGS\t: %02X\n", totalMem[i] );
//					break;
//				case 0x5C3D :
//					printf( "ERR-SP\t: %02X %02X\n", totalMem[i], totalMem[i++] );
//					break;
//				case 0x5C4F :
//					printf( "CHANS\t: %02X %02X\n", totalMem[i], totalMem[i++] );
//					break;
//				case 0x5C51 :
//					printf( "CURCHL\t: %02X %02X\n", totalMem[i], totalMem[i++] );
//					break;
//				case 0x5C53 :
//					printf( "PROG\t: %02X %02X\n", totalMem[i], totalMem[i++] );
//					break;
//				case 0x5C57:
//					printf( "DATADD\t: %02X %02X\n", totalMem[i], totalMem[i++] );
//					break;
//				case 0x5C6A :
//					printf( "FLAGS2\t: %02X\n", totalMem[i] );
//					break;
//				case 0x5C6B :
//					printf( "DF-SZ\t: %02X\n", totalMem[i] );
//					break;
//				case 0x5C7F :
//					printf( "P-POSN\t: %02X\n", totalMem[i] );
//					break;
//				case 0x5CB2 :
//					printf( "RAM-TOP\t: %02X %02X\n", totalMem[i], totalMem[i++] );
//					break;
//				case 0x5CB4 :
//					printf( "P-RAMT\t: %02X %02X\n", totalMem[i], totalMem[i++] );
//					break;
//				default:
//				break;
//			}
//		}
//	}

}

