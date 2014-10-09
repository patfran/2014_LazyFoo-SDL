/*
//
//Testing to see if I get the tick times
//

//Always include headers!
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <string>
#include <sstream>
#include <iostream>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

SDL_Surface* screen = NULL;
SDL_Surface* background = NULL;
SDL_Surface* startStop = NULL;
SDL_Surface* seconds = NULL;

TTF_Font* font = NULL;
SDL_Color textColor = { 0, 0, 0 };

SDL_Event event;

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
	SDL_WM_SetCaption( "time testing" , NULL );

	//If everything initialized alright
	return true;
}

bool load_files()
{
	//Load up the images
	background = load_image( "images/shopmart.bmp" );

	//Open up the font you want to use
	font = TTF_OpenFont( "lazy.ttf", 28 );

	//If there was a problem oading the background...
	if( background == NULL ){
		return false;
	}

	//If there was a problem loading the font...
	if( font == NULL ){
		return false;
	}

	return true;
}

void clean_up() //for every surface, need to change
{
	//Freee the surfaces!
	SDL_FreeSurface( background );
	SDL_FreeSurface( startStop );

	//Don't forget to close the font that was used!
	TTF_CloseFont( font );

	//Quit the SDL_ttf
	TTF_Quit();

	//Quit SDL
	SDL_Quit();
}

//----------------------------------------------------
int main( int argc, char* args[] )
{
	
	bool quit = false; //The quit flag. If it's true, then let's quit.

	Uint32 start = 0; //timer starting time
	bool running = true; //timer start/stop flag

	if( init() == false){ //Initialize the program
		return 1;
	}

	//Load up the files
	if( load_files() == false ){
		return 1;
	}

	//----------------
	
	startStop = TTF_RenderText_Solid( font, "Press S to start/stop the time", textColor );
	start = SDL_GetTicks();

	//So while the user didn't quit, keep it running
	while( quit == false ){
		while( SDL_PollEvent( &event ) ){ //And while there are events to handle
			if( event.type == SDL_KEYDOWN ){
				if( event.key.keysym.sym == SDLK_s ){ //if the s key was pressed
					if( running == true ){
						running = false; //stop the timer
						start = 0;
					} else {
						running = true; //start the timer up
						start = SDL_GetTicks();
					}
				}
			}

			//If the user wants to close the window
			else if( event.type == SDL_QUIT ){
				quit = true; //Quit the program
			}
		}

		apply_surface( 0, 0, background, screen );
		apply_surface( ( SCREEN_WIDTH - startStop->w ) / 2, 200, startStop, screen ); //startStop is message

		if( running == true )
		{
			//change the timer into a string
			std::stringstream time;
			time << "Timer: " << SDL_GetTicks() - start;

			seconds = TTF_RenderText_Solid( font, time.str().c_str(), textColor ); //render time surface
			apply_surface( ( SCREEN_WIDTH - seconds->w ) / 2, 50, seconds, screen ); //apply time surface
			SDL_FreeSurface( seconds ); //free up the time surface
		}
				
		//Update the screen
		if( SDL_Flip( screen ) == -1 ){
			return 1;
		}
	}
	//Always clean up the memory by free images
	clean_up();
	return 0;
}

*/