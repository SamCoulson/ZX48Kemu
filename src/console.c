#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include "../include/gui_ncurses.h"
#include "../include/console.h"
#include "../include/cpu.h"
#include "../include/memory.h"
#include "../include/util_bit_operations.h"

int key = 0;
char key_pressed;
void console(){

	// Curses must be initialised first
//  initscr();
//  // Disable buffering so that keys presses are sent straight away not at new line
//  cbreak();
//  // Disable echo so that keys are not printed back to the screen
//  noecho();
//	// Make getch a non blocking called
//	nodelay(stdscr, TRUE);
//
//  // Get the terminal window size
//  int h, w;
//  getmaxyx(stdscr, h, w);
//
//  // A refresh of stdscr must be called before windows can be drawn
//  refresh();
//
//  // Create window
//  WINDOW* win = createWindow(20, 60, 1, 1, "CPU Registers");
//
//	// Print out all cpu register values
//	mvwprintw(win, 3, 1, "AF: %02x %02x", *reg->a, *reg->f);
//	mvwprintw(win, 3, 15, "AF': %02x %02x",*reg->alta, *reg->altf );
//	mvwprintw(win, 4, 1, "BC: %02x %02x", *reg->b, *reg->c );
//	mvwprintw(win, 4, 15, "BC': %02x %02x", *reg->altb, *reg->altc );
//	mvwprintw(win, 5, 1, "DE: %02x %02x", *reg->d, *reg->e );
//	mvwprintw(win, 5, 15, "DE': %02x %02x", *reg->altd, *reg->alte );
//
//	mvwprintw(win, 6, 1, "HL: %02x %02x", *reg->h, *reg->l );
//	mvwprintw(win, 6, 15, "HL': %02x %02x", *reg->alth, *reg->altl );
//
//	mvwprintw(win, 8, 1, "SP: %04x", *reg->sp );
//	mvwprintw(win, 9, 1, "PC: %04x", *reg->pc );
//
//	mvwprintw(win, 8, 15, "IX: %04x", *reg->ix );
//	mvwprintw(win, 9, 15, "IY: %04x", *reg->iy );
//
//	mvwprintw(win, 11, 1, "I: %02x", *reg->i );
//	mvwprintw(win, 11, 15, "R: %02x", *reg->r );
//
//	mvwprintw(win, 13, 1, "EFF 1: %d", *reg->iff2 );
//	mvwprintw(win, 13, 15, "EFF 2: %d", *reg->iff1 );
//
//	// Window must be refreshed before text will appear
//	wrefresh(win);
//
  	// Wait for input from keyboard or mouse


  // Must be called to restore terminal settings
  //endwin();

//	static uint8_t stepMode = 1;


//	static int mode = 0;

//	system("cls");

//	if( *reg->pc == 0x18E1 )
//		stepMode = 1;
//
//	if( mode == 0 ){
//
//		printf("\nMain registers\tAlternate registers\n");
//		printf("AF: %02x %02x", *reg->a, *reg->f );
//		printf("\tAF': %02x %02x\n",*reg->alta, *reg->altf );
//		printf("BC: %02x %02x", *reg->b, *reg->c );
//		printf("\tBC': %02x %02x\n", *reg->altb, *reg->altc );
//		printf("DE: %02x %02x", *reg->d, *reg->e );
//		printf("\tDE': %02x %02x\n", *reg->altd, *reg->alte );
//
//		printf("HL: %02x %02x", *reg->h, *reg->l );
//		printf("\tHL': %02x %02x\n", *reg->alth, *reg->altl );
//
//		printf("\nSP: %04x", *reg->sp );
//		printf( "\nPC: %04x\n", *reg->pc );
//
//		printf("\nIX: %04x", *reg->ix );
//		printf("\tIY: %04x", *reg->iy );
//
//		printf("\nI: %02x", *reg->i );
//		printf("\t\tR: %02x\n", *reg->r );
//
//		printf("\nEFF 1: %d", *reg->iff2 );
//		printf("\tEFF 2: %d\n", *reg->iff1 );
//
//		printf("\n*** Flags ***\n");
//		printf("S:%d Z:%d -:%d H:%d -:%d P/V:%d N:%d C:%d",
//				getBit( reg->f, 7 ),
//				getBit( reg->f, 6 ),
//				getBit( reg->f, 5 ),
//				getBit( reg->f, 4 ),
//				getBit( reg->f, 3 ),
//				getBit( reg->f, 2 ),
//				getBit( reg->f, 1 ),
//				getBit( reg->f, 0 ) );
//
//		printf( "\n\n*** Stack ***\n" );
//
//		for( int i = 0xFF56; i > 0xFF35; i-- ){
//			printf("\n%X %X", i, totalMem[i] );
//			if( *reg->sp == i ){
//				printf( "\t<" );
//			}
//
//		}
//	printf("\n" );
//	}
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
//
	int key = getch();

	//if( newkey != key ) key = newkey;


	//if( key != ERR || stepMode ){

		printf("key press = %d\n", key);

		if( key == 113 ){
			exit(1); // key q
		}else if( key == 115 ){
			mode = 0; // key s
		}else if( key == 108 ) {
			mode = 4; // l key
		}else if( key == 109 ){
			mode = 1; // key m
		}else if( key == 114 ){
//			stepMode = stepMode ? 0 : 1; // r
		}else if( key == 118 ){
			mode = 2;
		}else if( key == 107 ){
			mode = 3; // k key
		}
	//}
}

