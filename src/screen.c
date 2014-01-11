#include "../include/screen.h"
#include "../include/memory.h"

SDL_Surface* screen = NULL;

int initSDL(){

	//Start SDL
	int result = SDL_Init( SDL_INIT_VIDEO );

	if(result != 0)
	{
		printf("Could not initialise SDL\n");
		return 1;
	}

	 //Set up screen
	screen = SDL_SetVideoMode( 256, 192, 8, SDL_HWSURFACE );

	//Apply image to screen
	//SDL_BlitSurface( hello, NULL, screen, NULL );

	SDL_FillRect( SDL_GetVideoSurface(), NULL, SDL_MapRGB( screen->format, 0, 0, 0 )  );

	//Update Screen
	SDL_Flip( screen );

	return 0;
}

void updateScreen(){

	// Row counter
	uint8_t column = 0, row = 0;
	
	int x = 0, y = 0; // Pixel coords

	int scale = 8; // Scale for different screen ratio's.

	uint16_t vidWidth = 0x00;
	uint16_t vidheight = 0x00;
	uint16_t vidStart = 0x4000;
	uint16_t vidBufLoc = vidStart; // Current address into memory buffer	
	
	int bit = 0;
		
	// Determine picel colors
	Uint32 foreground = SDL_MapRGB( screen->format, 255, 255, 255 ); 
	Uint32 background = SDL_MapRGB( screen->format, 0, 0, 0 );

	// Update 256 x 192 pixels  
	for( y = 0; y < 192; ){
		for( x = 0; x < 256; ){
			// Read spec video mem buffer and get pixel status + determine color
			uint8_t pixel = totalMem[vidBufLoc];

			for( bit = 0; bit < 8; bit++ ){
				// Set on pixel to forground color
				if( pixel & (1 << (bit) ) ){
					putColor( screen, x, y, foreground );
				}else{
					putColor( screen, x, y, background );
				}
				x++;
			}
			vidBufLoc++;	
		}	
		y++; //  Progress 1 pixel down
		// If y < current 8x8 block reset 
		if( y < column ){
			// Reset the attribute location pointer to beginning of line to retain 
			// PAPER and INK attiributes.	
			//vidBufLoc = 0x4000;
		}else{
			column += 8; // Set the next y pos to 8 pixel down, end of next 8x8 block
			// Move Attribute buffer pointer to next line
			// Will need to multipy row number by 32 to determine the location. 	
		}
	}			
	
	SDL_Flip( screen );
}

void putColor(SDL_Surface *surface, int x, int y, Uint32 color){

	int bpp = surface->format->BytesPerPixel;

	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch( bpp ) 
	{
    case 1:
        *p = color;
        break;

    case 2:
        *(Uint16 *)p = color;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) 
		{
            p[0] = (color >> 16) & 0xff;
            p[1] = (color >> 8) & 0xff;
            p[2] = color & 0xff;
        } 
		else 
		{
            p[0] = color & 0xff;
            p[1] = (color >> 8) & 0xff;
            p[2] = (color >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = color;
        break;
	}
}


