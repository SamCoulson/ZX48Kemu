#include <stdlib.h>
#include "raylib.h"
#include "../include/raygui.h"
#include "../include/screen.h"
#include "../include/memory.h"
#include "../include/cpu.h"
//#include "../include/ula.h"
#include "../include/util_bit_operations.h"

#define SCREEN_WIDTH 256
#define SCRREN_HEIGHT 192
#define SCREEN_X_START 50
#define SCREEN_Y_START 50

extern bool paused;
extern bool shouldUpdateScreen;

int initWindow(){

	const int screenWidth = 640;
	const int screenHeight = 480;

	//initSDL();
	InitWindow(screenWidth, screenHeight, "ZX Spectrum emulator");
	BeginDrawing();
		ClearBackground(RAYWHITE);
	EndDrawing();                     
/*
	// Test 8x8 block for screen
	totalMem[ 0x4000 ] = 0x00;
	totalMem[ 0x4100 ] = 0x3C;
	totalMem[ 0x4200 ] = 0x42;
	totalMem[ 0x4300 ] = 0x04;
	totalMem[ 0x4400 ] = 0x08;
	totalMem[ 0x4500 ] = 0x00;
	totalMem[ 0x4600 ] = 0x08;
	totalMem[ 0x4700 ] = 0x00;

	// Second third
	totalMem[ 0x4800 ] = 0xFF;
	totalMem[ 0x4900 ] = 0xFF;
	totalMem[ 0x4a00 ] = 0xFF;
	totalMem[ 0x4b00 ] = 0xFF;
	totalMem[ 0x4c00 ] = 0xFF;
	totalMem[ 0x4d00 ] = 0xFF;
	totalMem[ 0x4e00 ] = 0xFF;
	totalMem[ 0x4f00 ] = 0xFF;

	// Lower third
	totalMem[ 0x5000 ] = 0xFF;
	totalMem[ 0x5100 ] = 0xFF;
	totalMem[ 0x5200 ] = 0xFF;
	totalMem[ 0x5300 ] = 0xFF;
	totalMem[ 0x5400 ] = 0xFF;
	totalMem[ 0x5500 ] = 0xFF;
	totalMem[ 0x5600 ] = 0xFF;
	totalMem[ 0x5700 ] = 0xFF;
	
	totalMem[ 0x5020 ] = 0xFF;
	totalMem[ 0x5120 ] = 0xFF;
	totalMem[ 0x5220 ] = 0xFF;
	totalMem[ 0x5320 ] = 0xFF;
	totalMem[ 0x5420 ] = 0xFF;
	totalMem[ 0x5520 ] = 0xFF;
	totalMem[ 0x5620 ] = 0xFF;
	totalMem[ 0x5720 ] = 0xFF;

	totalMem[ 0x5040 ] = 0xFF;
	totalMem[ 0x5140 ] = 0xFF;
	totalMem[ 0x5240 ] = 0xFF;
	totalMem[ 0x5340 ] = 0xFF;
	totalMem[ 0x5440 ] = 0xFF;
	totalMem[ 0x5540 ] = 0xFF;
	totalMem[ 0x5640 ] = 0xFF;
	totalMem[ 0x5740 ] = 0xFF;

	totalMem[ 0x5060 ] = 0xFF;
	totalMem[ 0x5160 ] = 0xFF;
	totalMem[ 0x5260 ] = 0xFF;
	totalMem[ 0x5360 ] = 0xFF;
	totalMem[ 0x5460 ] = 0xFF;
	totalMem[ 0x5560 ] = 0xFF;
	totalMem[ 0x5660 ] = 0xFF;
	totalMem[ 0x5760 ] = 0xFF;

	totalMem[ 0x5080 ] = 0xFF;
	totalMem[ 0x5180 ] = 0xFF;
	totalMem[ 0x5280 ] = 0xFF;
	totalMem[ 0x5380 ] = 0xFF;
	totalMem[ 0x5480 ] = 0xFF;
	totalMem[ 0x5580 ] = 0xFF;
	totalMem[ 0x5680 ] = 0xFF;
	totalMem[ 0x5780 ] = 0xFF;

	totalMem[ 0x50A0 ] = 0xFF;
	totalMem[ 0x51A0 ] = 0xFF;
	totalMem[ 0x52A0 ] = 0xFF;
	totalMem[ 0x53A0 ] = 0xFF;
	totalMem[ 0x54A0 ] = 0xFF;
	totalMem[ 0x55A0 ] = 0xFF;
	totalMem[ 0x56A0 ] = 0xFF;
	totalMem[ 0x57A0 ] = 0xFF;


	totalMem[ 0x50C0 ] = 0xFF;
	totalMem[ 0x51C0 ] = 0xFF;
	totalMem[ 0x52C0 ] = 0xFF;
	totalMem[ 0x53C0 ] = 0xFF;
	totalMem[ 0x54C0 ] = 0xFF;
	totalMem[ 0x55C0 ] = 0xFF;
	totalMem[ 0x56C0 ] = 0xFF;
	totalMem[ 0x57C0 ] = 0xFF;

	totalMem[ 0x50E0 ] = 0xFF;
	totalMem[ 0x51E0 ] = 0xFF;
	totalMem[ 0x52E0 ] = 0xFF;
	totalMem[ 0x53E0 ] = 0xFF;
	totalMem[ 0x54E0 ] = 0xFF;
	totalMem[ 0x55E0 ] = 0xFF;
	totalMem[ 0x56E0 ] = 0xFF;
	totalMem[ 0x57E0 ] = 0xFF;
*/

	return 0;
}

void updateScreen(){

	BeginDrawing();

		//ClearBackground(RAYWHITE);
		if (shouldUpdateScreen)
		{
		ClearBackground(RAYWHITE);
			int x = SCREEN_X_START, y = SCREEN_Y_START; // Pixel coords

			uint16_t vidStart = 0x4000;
			uint16_t vidBufLoc = vidStart; // Current address into memory buffer	
			
			int bit = 0;

			// Set the whole screen to what ever border colour is

			// Loop through the thirds
			for( int i = 0; i < 3; i++ ){
				// Loop through the 8 lines of each row of this third
				for( int j = 0; j < 8; j++){	
					vidBufLoc = vidStart;
					//printf("\nvidStart = %X\n", vidStart);

					// Loop through the 8 rows
					for( int k = 0; k < 8; k++ ){
						x = SCREEN_X_START;	
						//printf("\nvidBufLoc = %X\n", vidBufLoc);
						for( int l = 0; l < 32; l++ ){
							// Read spec video mem buffer and get pixel status + determine color
							uint8_t pixel = totalMem[vidBufLoc];

							for( bit = 7; bit > -1; bit-- ){
								// Set on pixel to forground color
								if( pixel & (1 << (bit) ) ){
									DrawPixel(x, y, BLACK);
								}else{
									DrawPixel(x, y, WHITE);
								}
								x++;
							}
							vidBufLoc++;		
						}
						vidBufLoc+=224; // Advance to next set of bytes for next row 
						y++;
			
					}
					vidStart += 32; // Advance start offset by 32 bytes for next pixel line
					
				}
				vidStart += 1792; // Move pointer forwards to start of the next set of bytes for the next third 
			}
			shouldUpdateScreen = false;
		}
		const int reg_box_x = 350;
		const int reg_box_y = 20;
		const int reg_box_w = 175;
		const int reg_box_h = 160;

		const int flags_box_x = 350; 
		const int flags_box_y = 200;
		const int flags_box_w = 175;
		const int flags_box_h = 30;

		
		const int stack_box_x = 535;
		const int stack_box_y = 20;
		const int stack_box_w = 70;
		const int stack_box_h = 370;

		const int font_size = 10;

		GuiGroupBox((Rectangle){ reg_box_x, reg_box_y, reg_box_w, reg_box_h }, "REGISTERS");

		DrawText(TextFormat("AF: %02X %02X", *reg->a, *reg->f), reg_box_x+10, 30, font_size, BLACK);
		DrawText(TextFormat("AF': %02X %02X", *reg->alta, *reg->altf), reg_box_x+75, 30, font_size, BLACK);
		DrawText(TextFormat("BC: %02X %02X", *reg->b, *reg->c), reg_box_x+10, 40, font_size, BLACK);
		DrawText(TextFormat("BC': %02X %02X", *reg->altb, *reg->altc), reg_box_x+75, 40, font_size, BLACK);
		DrawText(TextFormat("DE: %02X %02X", *reg->d, *reg->e), reg_box_x+10, 50, font_size, BLACK);
		DrawText(TextFormat("DE': %02X %02X", *reg->altd, *reg->alte), reg_box_x+75, 50, font_size, BLACK);

		DrawText(TextFormat("HL: %02X %02X", *reg->h, *reg->l), reg_box_x+10, 70, 10, BLACK);
		DrawText(TextFormat("HL': %02X %02X", *reg->alth, *reg->altl), reg_box_x+75, 70, 10, BLACK);

		DrawText(TextFormat("SP: %04X", *reg->sp), reg_box_x+10, 90, 10, BLACK);
		DrawText(TextFormat("PC: %04X", *reg->pc), reg_box_x+75, 90, 10, BLACK);

		DrawText(TextFormat("IX: %04X", *reg->ix), reg_box_x+10, 110, 10, BLACK);
		DrawText(TextFormat("IY: %04X", *reg->ix), reg_box_x+75, 110, 10, BLACK);

		DrawText(TextFormat("I: %02X", *reg->i), reg_box_x+10, 130, 10, BLACK);
		DrawText(TextFormat("R: %02X", *reg->r), reg_box_x+75, 130, 10, BLACK);

		DrawText(TextFormat("EFF 1: %d", *reg->iff1), reg_box_x+10, 150, 10, BLACK);
		DrawText(TextFormat("EFF 2: %d", *reg->iff2), reg_box_x+75, 150, 10, BLACK);

		GuiGroupBox((Rectangle){ flags_box_x, flags_box_y, flags_box_w, flags_box_h }, "FLAGS");

		DrawText(TextFormat("S:%d Z:%d -:%d H:%d -:%d P/V:%d N:%d C:%d",
				getBit( reg->f, 7 ),
				getBit( reg->f, 6 ),
				getBit( reg->f, 5 ),
				getBit( reg->f, 4 ),
				getBit( reg->f, 3 ),
				getBit( reg->f, 2 ),
				getBit( reg->f, 1 ),
				getBit( reg->f, 0 ) ), flags_box_x+10, flags_box_y+10, 10, BLACK);

		GuiGroupBox((Rectangle){ stack_box_x, stack_box_y, stack_box_w, stack_box_h }, "STACK");

		int k = 30;
		for( int i = 0xFF56; i > 0xFF35; i-- ){
			DrawText(TextFormat("%X %X", i, totalMem[i]), stack_box_x+10, k, 10, BLACK);
			k+=10;
		}

		GuiSetState(STATE_NORMAL);
		if (GuiButton((Rectangle){ 50, 400, 40, 20}, "Pause"))
		{
			if(paused == true){
				paused = false;
			}else{
				paused = true;
			}
		}

	EndDrawing();
}


//	// These are global variables in the 48k rom so not internal to the cpu not sure what to do with these super useful but
//	// but can only be used when 48k rom is loaded.
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





























