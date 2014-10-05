//Finally! Text stuff!
//http://lazyfoo.net/SDL_tutorials/lesson07/index.php


//ALWAYS INCLUDE HEADERS
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <string>

//INCLUDE SCREEN RES
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;


//The surfaces (areas of drawable graphical memory)
SDL_Surface* screen = NULL;
SDL_Surface* background = NULL;
SDL_Surface* message = NULL;

//Set the event structure
SDL_Event event;

//Set up the font you're going to use
TTF_Font* font = NULL;

//Set up the color of the font
SDL_Color textColor = { 0, 0, 0 };

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
	//Initialize all SDL subsystems
	if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
	{
		return false;
	}

	//Set up the screen
	screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

	//If there was an error in setting up the screen, return false
	if( screen == NULL )
	{
		return false;
	}

	//Initialize the SDL_ttf for text and fonts
	if( TTF_Init() == -1 )
	{
		return false;
	}

	//Set the window title caption thing at the top
	SDL_WM_SetCaption( "TTF testing!", NULL );

	//If everything initialized alright
	return true;
}


bool load_files()
{
	//Load up the background image
	background = load_image( "images/background.bmp" );

	//Open up the font you want to use
	font = TTF_OpenFont( "lazy.ttf", 28 );

	//If there was a problem oading the background...
	if( background == NULL )
	{
		return false;
	}

	//If there was a problem loading the font...
	if( font == NULL )
	{
		return false;
	}

	return true;
}

void clean_up()
{
	//Freee the surfaces!
	SDL_FreeSurface( background );
	SDL_FreeSurface( message );

	//Don't forget to close the font that was used!
	TTF_CloseFont( font );

	//Quit the SDL_ttf
	TTF_Quit();

	//Quit SDL
	SDL_Quit();
}

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

	//----------------
	//Render the text
	message = TTF_RenderText_Solid( font, "The quick brown fence jumps over the doggy dog", textColor );

	//If there was an error rendering the text...
	if( message == NULL )
	{
		return 1;
	}

	//Apply the images to the screen!
	apply_surface( 0, 0, background, screen );
	apply_surface( 0, 150, message, screen );
	//----------------

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