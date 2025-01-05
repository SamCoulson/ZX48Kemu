#include "../include/screen.h"
#include "../include/debugger.h"
#include "../include/disassembler.h"
#include <stdint.h>
#include <stdio.h>

#define SCREEN_WIDTH 256
#define SCRREN_HEIGHT 192
#define SCREEN_X_START 20
#define SCREEN_Y_START 20

#define TOTAL_SCREEN_PIXELS 49152

const int registers_box_x = 300;
const int registers_box_y = 20;
const int registers_box_w = 175;
const int registers_box_h = 160;

const int flags_box_x = 300; 
const int flags_box_y = 200;
const int flags_box_w = 175;
const int flags_box_h = 30;

const int stack_box_x = 485;
const int stack_box_y = 20;
const int stack_box_w = 70;
const int stack_box_h = 360;

const int instructions_box_x = 565;
const int instructions_box_y = 20;
const int instructions_box_w = 110;
const int instructions_box_h = 360;

const int breakpoints_box_x = 300;
const int breakpoints_box_y = 250;
const int breakpoints_box_w = 175;
const int breakpoints_box_h = 130;

const int font_size = 10;

typedef struct 
{
	int x;
	int y;
	uint16_t index;
	Color color;
}PixelValue;

PixelValue pixel_values[TOTAL_SCREEN_PIXELS]; 

void test_video();
// this methiod should live in ULA.c
void build_video_mem_map()
{
	// Memory divided in to three blocks of 64 rows each, each block has 8 row groups.
	// The screen is 32x24 grid.


	int index = 0;
	int x = SCREEN_X_START, y = SCREEN_Y_START; // Pixel coords

	uint16_t vidStart = 0x4000;
	uint16_t vidBufLoc = vidStart; // Current address into memory buffer	
	
	int bit = 0;

	// Set the whole screen to what ever border colour is

	// Loop through the thirds
	for( int i = 0; i < 3; i++ ){
		// Loop through the 8 lines of each row of this third, each line is 8 pixels high
		for( int j = 0; j < 8; j++){	
			vidBufLoc = vidStart;
			//printf("\nvidStart = %X\n", vidStart);

			// Loop through the 8 pixel lines of the current row
			for( int k = 0; k < 8; k++ ){
				x = SCREEN_X_START;	
				//printf("\nvidBufLoc = %X\n", vidBufLoc);
				//32 blocks of 8 bits per row.
				for( int l = 0; l < 32; l++ ){
					// Read spec video mem buffer and get pixel status + determine color
					uint8_t pixel = totalMem[vidBufLoc];

					// loop through each bit in the byte (bit 7 MSB is left most pixel and bit 0 LSD right most pixel)
					for( bit = 7; bit > -1; bit-- )
					{
						pixel_values[index].x = x; 
						pixel_values[index].y = y; 

						// Set on pixel to forground color
						if( pixel & (1 << (bit) ) ){
					//		pixel_values[index].color = BLACK;
							DrawPixel(x, y, BLACK);
						}else{
					//		pixel_values[index].color = WHITE; 
							DrawPixel(x, y, WHITE);
						}

						pixel_values[index].index = vidBufLoc;

						index++;
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
}

int initWindow(){

	const int screenWidth = 685;
	const int screenHeight = 480;

	//initSDL();
	InitWindow(screenWidth, screenHeight, "ZX Spectrum emulator");
	BeginDrawing();
		ClearBackground(RAYWHITE);
	EndDrawing();                     
	
	build_video_mem_map();

	return 0;
}
bool textBoxEditMode = false;

char *buttonText = "Pause";

void updateScreen(){

	BeginDrawing();

		ClearBackground(RAYWHITE);
		
		test_video();	
//	//	if (shouldUpdateScreen)
//	//	{
//	//		shouldUpdateScreen = false;
	//	
	//		for (int i = 0; i < TOTAL_SCREEN_PIXELS - 1; i++)
	//		{
	////			printf("%d %d %d\n", pixel_values[i].x, pixel_values[i].y, pixel_values[i].color);

	//			uint8_t pixel = totalMem[pixel_values[i].index];

	//			for( int bit = 7; bit > -1; bit-- )
	//			{
	//				// Set on pixel to forground color
	//				if( pixel & (1 << (bit) ) ){
	//					pixel_values[i].color = BLACK;
	//				}else{
	//					pixel_values[i].color = WHITE; 
	//				}

	//				DrawPixel(pixel_values[i].x, pixel_values[i].y, pixel_values[i].color);
	//				//printf("x = %d, Y = %d", pixel_values[i].x, pixel_values[i].y);	
	//				
	//			}
	//			
	//		}
//	//	}

		build_video_mem_map();

		GuiGroupBox((Rectangle){ registers_box_x, registers_box_y, registers_box_w, registers_box_h }, "REGISTERS");

		DrawText(TextFormat("AF: %02X %02X", *reg->a, *reg->f), registers_box_x+10, 30, font_size, BLACK);
		DrawText(TextFormat("AF': %02X %02X", *reg->alta, *reg->altf), registers_box_x+75, 30, font_size, BLACK);
		DrawText(TextFormat("BC: %02X %02X", *reg->b, *reg->c), registers_box_x+10, 40, font_size, BLACK);
		DrawText(TextFormat("BC': %02X %02X", *reg->altb, *reg->altc), registers_box_x+75, 40, font_size, BLACK);
		DrawText(TextFormat("DE: %02X %02X", *reg->d, *reg->e), registers_box_x+10, 50, font_size, BLACK);
		DrawText(TextFormat("DE': %02X %02X", *reg->altd, *reg->alte), registers_box_x+75, 50, font_size, BLACK);

		DrawText(TextFormat("HL: %02X %02X", *reg->h, *reg->l), registers_box_x+10, 70, 10, BLACK);
		DrawText(TextFormat("HL': %02X %02X", *reg->alth, *reg->altl), registers_box_x+75, 70, 10, BLACK);

		DrawText(TextFormat("SP: %04X", *reg->sp), registers_box_x+10, 90, 10, BLACK);
		DrawText(TextFormat("PC: %04X", *reg->pc), registers_box_x+75, 90, 10, BLACK);

		DrawText(TextFormat("IX: %04X", *reg->ix), registers_box_x+10, 110, 10, BLACK);
		DrawText(TextFormat("IY: %04X", *reg->ix), registers_box_x+75, 110, 10, BLACK);

		DrawText(TextFormat("I: %02X", *reg->i), registers_box_x+10, 130, 10, BLACK);
		DrawText(TextFormat("R: %02X", *reg->r), registers_box_x+75, 130, 10, BLACK);

		DrawText(TextFormat("EFF 1: %d", *reg->iff1), registers_box_x+10, 150, 10, BLACK);
		DrawText(TextFormat("EFF 2: %d", *reg->iff2), registers_box_x+75, 150, 10, BLACK);

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
			DrawText(TextFormat("%04X %02X", i, totalMem[i]), stack_box_x+10, k, 10, BLACK);
			// need to add arrow to show where the sp is currently pointing too
			k+=10;
		}

		GuiGroupBox((Rectangle){ instructions_box_x, instructions_box_y, instructions_box_w, instructions_box_h }, "INSTRUCTIONS");
		int instruction_v_spacing = 30;
		for( int i = 0; i < sizeof(disass_instructions) / sizeof(char*); i++)
		{
			DrawText(TextFormat("%s", disass_instructions[i]), instructions_box_x+10, instruction_v_spacing, 10, BLACK);
			// need to add arrow to show where the sp is currently pointing too
			instruction_v_spacing+=10;
		}


		GuiGroupBox((Rectangle){ breakpoints_box_x, breakpoints_box_y, breakpoints_box_w, breakpoints_box_h}, "BREAKPOINTS");

		GuiLabel((Rectangle){breakpoints_box_x+10, breakpoints_box_y+5, 30, 30 }, "1. 0x");
		if (GuiTextBox((Rectangle){ breakpoints_box_x+35, breakpoints_box_y+10, 50, 20 }, breakpoint, 64, textBoxEditMode)) 
		{
			textBoxEditMode = !textBoxEditMode;
			printf("*******************************breakpoint set to %s and PC is %04X\n", breakpoint, *reg->pc);
		}
		
		GuiSetState(STATE_NORMAL);
		if (GuiButton((Rectangle){ 300, 400, 60, 30}, buttonText))
		{
			if(paused == true){
				paused = false;

				initDisassInstructionsBuffer();
			}else{
				paused = true;

				uint16_t prev_instruct_start = *reg->pc - DISASS_INSTRUCT_BUFFER_SIZE;

				for(int i = 0; i < DISASS_INSTRUCT_BUFFER_SIZE; i++)
				{
					//printf("%04X\n", totalMem[prev_instruct_start]);
					disass_instructions[i] = disassemble_single_byte_opcode(&totalMem[prev_instruct_start]);
					prev_instruct_start++;
				}
			}
		}

		GuiSetState(STATE_NORMAL);
		if (GuiButton((Rectangle){ 420, 400, 60, 30}, "Step"))
		{
			// only allow stepping from paused or start
			if(paused || (paused && *reg->pc == 0000))
			{
				stepping = true;	
			}
		}

		GuiSetState(STATE_NORMAL);
		if (GuiButton((Rectangle){ 300, 440, 60, 30}, "Reset"))
		{
			// TODO also reset registers
			*reg->pc = 0000;
		}


		if(paused)
		{
	
			buttonText = "Resume";
		}
		else
		{
			buttonText = "Pause";
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

void test_video()
{


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

}


























