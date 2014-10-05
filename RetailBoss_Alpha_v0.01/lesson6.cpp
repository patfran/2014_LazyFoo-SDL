//Clip Blitting and Sprite Sheets
//http://lazyfoo.net/SDL_tutorials/lesson06/index.php


//Headers. ALWAYS NEED HEADERS
#include "SDL.h"
#include "SDL_image.h"
#include <string>

//Screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;


//The sufaces
SDL_Surface* dots = NULL;
SDL_Surface* screen = NULL;

//The event structure
SDL_Event event;

//The portions of the sprite map to be blitted
SDL_Rect clip[ 4 ];

//----------------------------------------------------

SDL_Surface* load_image( std::string filename )
{
	//This is the image that's loaded
	SDL_Surface* loadedImage = NULL;

	//This is the image that's optimized
	SDL_Surface* optimizedImage = NULL;

	//Load the image
	loadedImage = IMG_Load( filename.c_str() );

	//If there is an image loaded...
	if( loadedImage != NULL )
	{
		//Create an optimized surface
		optimizedImage = SDL_DisplayFormat( loadedImage );

		//Free the old surface
		SDL_FreeSurface( loadedImage );

		//If the surface was successfully optimized
		if( optimizedImage != NULL )
		{
			//Set the color key surface
			//The 0xFF etc is the color you want to be invisible/transparent
			SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );
		}
	}
	//Return the final optimized image
	return optimizedImage;
}

//----------------------------------------------------

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL)
{
	//Holds offsets
	SDL_Rect offset;

	//Get offsets
	offset.x = x;
	offset.y = y;

	//Blit
	SDL_BlitSurface( source, clip, destination, &offset );
}

bool init()
{
	//Initialze all the SDL stuff
	if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
	{
		return false;
	}

	//Set up the actual screen
	screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

	//If there was an error setting up the screen...
	if( screen == NULL )
	{
		return false;
	}

	//Set the little title of the window at the top
	SDL_WM_SetCaption( "Split the dots!", NULL );

	//If everything's working as planned...
	return true;
}

bool load_files()
{
	//Load the sprite sheet
	dots = load_image( "images/dots.png" );

	//If there was a problem loading the sprite sheet...
	if( dots == NULL )
	{
		return false;
	}

	//If everything is alright...
	return true;
}

//A little function to do to clean up old stuff and quit
void clean_up()
{
	//Free the sprite map
	SDL_FreeSurface( dots );

	//Quit SDl when everything's cleaned up
	SDL_Quit();
}

//----------------------------------------------------
//THE MEAT OF THE PROGRAM

int main( int argc, char* args[] )
{
	//The quit flag. If it's true, then let's quit.
	bool quit = false;

	//Initialize the program
	if( init() == false)
	{
		return 1;
	}

	//Load up the files
	if( load_files() == false )
	{
		return 1;
	}

	//Clip range for top left sprite
	clip[ 0 ].x = 0;
	clip[ 0 ].y = 0;
	clip[ 0 ].w = 100;
	clip[ 0 ].h = 100;

	//Clip range for top right sprite
	clip[ 1 ].x = 100;
	clip[ 1 ].y = 0;
	clip[ 1 ].w = 100;
	clip[ 1 ].h = 100;

	//Clip range for bottom left sprite
	clip[ 2 ].x = 0;
	clip[ 2 ].y = 100;
	clip[ 2 ].w = 100;
	clip[ 2 ].h = 100;

	//Clip range for the bottom right
	clip[ 3 ].x = 100;
	clip[ 3 ].y = 100;
	clip[ 3 ].w = 100;
	clip[ 3 ].h = 100;

	//Fill the screen with white
	SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF) );

	//Apply the images to the screen
	apply_surface( 0, 0, dots, screen, &clip[ 0 ] );
	apply_surface( 540, 0, dots, screen, &clip[ 1 ] );
	apply_surface( 0, 380, dots, screen, &clip[ 2 ] );
	apply_surface( 540, 380, dots, screen, &clip[ 3 ] );

	//Update the screen
	if( SDL_Flip( screen ) == -1 )
	{
		return 1;
	}

	//So while the user didn't quit, keep it running
	while( quit == false )
	{
		//And while there are events to handle
		while( SDL_PollEvent( &event ) )
		{
			//If the user wants to close the window
			if( event.type == SDL_QUIT )
			{
				//Quit the program
				quit = true;
			}
		}
	}

	//Always clean up the memory by free images
	clean_up();

	return 0;
}