#include "../include/screen.h"
#include "../include/memory.h"

#define SCREEN_WIDTH 256
#define SCRREN_HEIGHT 192
#define SCREEN_X_START 50
#define SCREEN_Y_START 50


SDL_Window* screen = NULL;
SDL_Renderer* sdlRenderer = NULL;
SDL_Event event;

int initSDL(){

	//Start SDL
	int result = SDL_Init( SDL_INIT_VIDEO );

	if(result != 0)
	{
		printf("Could not initialise SDL\n");
		return 1;
	}

	 //Set up screen
	screen = SDL_CreateWindow("ZX48KEmu", 
			SDL_WINDOWPOS_UNDEFINED, 
			SDL_WINDOWPOS_UNDEFINED, 
			356, 292, 
			SDL_WINDOW_MAXIMIZED | SDL_WINDOW_OPENGL);

	sdlRenderer = SDL_CreateRenderer(screen, -1, 0);

	//Apply image to screen

	SDL_SetRenderDrawColor(sdlRenderer, 255, 255, 255, 255);
	SDL_RenderClear(sdlRenderer);
	SDL_RenderPresent(sdlRenderer);

	//Update Screen

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

	SDL_PollEvent( &event );

	// Row counter
	uint8_t column = 0, row = 0;	
	
	int x = SCREEN_X_START, y = SCREEN_Y_START; // Pixel coords

	int scale = 8; // Scale for different screen ratio's.
	
	uint16_t vidStart = 0x4000;
	uint16_t vidBufLoc = vidStart; // Current address into memory buffer	
	
	int bit = 0;
	int third = 0;
		
	// Determine picel colors
	//Uint32 foreground = SDL_MapRGB( screen->format, 0, 0, 0 ); 
	//Uint32 background = SDL_MapRGB( screen->format, 255, 255, 255 );

	// Set the whole screen to what ever border colour is
	SDL_RenderPresent( sdlRenderer );

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
							//putColor( screen, x, y, foreground );
							SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
							SDL_RenderDrawPoint(sdlRenderer, x, y);
						}else{

							SDL_SetRenderDrawColor(sdlRenderer, 255, 255, 255, 255);
							SDL_RenderDrawPoint(sdlRenderer, x, y);
							//putColor( screen, x, y, background );
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

	SDL_RenderPresent( sdlRenderer );

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


