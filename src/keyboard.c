#include "../include/keyboard.h"

// The keyboard can be directly accessed by the Z80 cpu, by way of the upper-half of the 8-15 bits
// of the address bus.  When it does this, a specific row of the 8 posible rows will be specified,
// the readKeyboard routine sets the appropriate bits corresponding to keys pressed and passes back the requested
// byte (row).
//
// The rows are as follows
//
// 1111 1110 1111 1110 = 0xFEFE Row 1
// 1111 1101 1111 1110 = 0xFDFE Row 2 
// 1111 1011 1111 1110 = 0xFBFE Row 3
// 1111 0111 1111 1110 = 0xF7FE Row 4
// 1110 1111 1111 1110 = 0xEFFE Row 5
// 			 0xDFFE Row 6
// 			 0xBFFE Row 7
// 			 0x7FFE Row 8

// Bytes to hold 8 rows, all initialised to 0xBF
uint8_t halfRows[8] = { [0 ... 7] = 0xBF };

// Sets the corresponding bits for key presses in the halfRows array
// gets called by the ULA or run depending on where best to call it from,
// the important thing is that all it does is acts as keybaord unit out side of the CPU.
void readKeyboard(){

	//if( event.type == SDL_KEYDOWN  ){
			
	//	switch( event.key.keysym.sym ){
			// Row 1
	//		case SDLK_z:
				//key = 0x41; 	
	//			break;
	//		case SDLK_x:
				//key = 0x41; 	
	//			break;
	//		case SDLK_c:
				//key = 0x41; 	
	//			break;
	//		case SDLK_v:
	//			halfRows[0] = 0x8F; 	
	//			break;
	//		default:
	//			break;
	//	}
	//}
//	halfRows[0] = 0x8F; // *** USE FOR DEBUG TO SIMULATE A KEY PRESS ***

}
