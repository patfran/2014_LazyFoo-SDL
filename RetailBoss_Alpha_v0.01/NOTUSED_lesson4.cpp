/*
//Event Driven Programming
//http://lazyfoo.net/SDL_tutorials/lesson04/index.php

//Headers
#include "SDL.h"
#include "SDL_image.h"
#include <string>

//Screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//The surfaces
SDL_Surface *image = NULL;
SDL_Surface *screen = NULL;

//Event structure that will be used
SDL_Event event;

SDL_Surface *load_image( std::string filename )
{
	//The image that's loaded
	SDL_Surface *loadedImage = NULL;

	//The optimized image that will be used
	SDL_Surface *optimizedImage = NULL;

	//Load the image
	loadedImage = IMG_Load( filename.c_str() );

	//If the image loaded...
	if( loadedImage != NULL )
	{
		//Create an optimized image
		optimizedImage = SDL_DisplayFormat( loadedImage );

		//Free the old image
		SDL_FreeSurface( loadedImage );
	}

	//Return the optimized image
	return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface *source, SDL_Surface *destination )
{
	//Temporary rectangle to hold the offsets
	SDL_Rect offset;

	//Get the offsets
	offset.x = x;
	offset.y = y;

	//Blit the surface
	SDL_BlitSurface( source, NULL, destination, &offset );
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

	//If there was an error in setting up the screen
	if( screen == NULL )
	{
		return false;
	}

	//Set the window caption
	SDL_WM_SetCaption( "Event testing!", NULL );

	//If everything intialized alright
	return true;
}

bool load_files()
{
	//Load the image
	image = load_image( "images/xout.png" );

	//If there was an error in loading the image
	if( image == NULL )
	{
		return false;
	}

	//If everything loaded fine
	return true;
}

void clean_up()
{
	//Free the image
	SDL_FreeSurface( image );

	//Quit SDL
	SDL_Quit();
}

//---------------------------------------------------------

int main( int argc, char* args[] )
{
	//Make sure the program waits for a quit
	bool quit = false;

	//Initialize
	if( init() == false )
	{
		return 1;
	}

	//Load the files
	if( load_files() == false )
	{
		return 1;
	}

	//Apply the surface to the screen
	apply_surface( 0, 0, image, screen );

	//Update the screen
	if( SDL_Flip( screen ) == -1 )
	{
		return 1;
	}

	//*** HERE'S THE MAIN MEAT OF THE FILE ***
	//(so I think the game is stuck in this loop forever until it
	//gets broken out of, aka when quit equals true)
	while( quit == false )
	{
		//While there's an event to handle
		while( SDL_PollEvent( &event ) )
		{
			//If the user has closed [X] the window...
			if( event.type == SDL_QUIT )
			{
				//Quit the program
				quit = true;
			}
		}
	}

	clean_up();
	return 0;
}

*/