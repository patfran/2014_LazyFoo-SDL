/*

//Keypress detections
//http://lazyfoo.net/SDL_tutorials/lesson08/index.php

//Always headers
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//The surfaces (areas of drawable graphical memory)
SDL_Surface* upMessage = NULL;
SDL_Surface* downMessage = NULL;
SDL_Surface* leftMessage = NULL;
SDL_Surface* rightMessage = NULL;
SDL_Surface* background = NULL;
SDL_Surface* message = NULL;
SDL_Surface* screen = NULL;

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
	SDL_WM_SetCaption( "Arrow key pressing!", NULL );

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
	SDL_FreeSurface( upMessage );
	SDL_FreeSurface( downMessage );
	SDL_FreeSurface( leftMessage );
	SDL_FreeSurface( rightMessage );
	SDL_FreeSurface( background );
	SDL_FreeSurface( message );

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
    //Quit flag, initialize, and load files
    bool quit = false;

    if( init() == false )
	{
        return 1;
    }

    if( load_files() == false )
	{
        return 1;
    }

	//Let's create the message surfaces
	upMessage = TTF_RenderText_Solid( font, "Up was pressed", textColor );
	downMessage = TTF_RenderText_Solid( font, "Down was pressed", textColor );
	leftMessage = TTF_RenderText_Solid( font, "Left was pressed", textColor );
	rightMessage = TTF_RenderText_Solid( font, "Right was pressed", textColor );

	if( upMessage == NULL || downMessage == NULL || leftMessage == NULL || rightMessage == NULL)
	{
		return 1;
	}

	//First apply the background
	apply_surface( 0, 0, background, screen );

	//While the user hasn't quit
	while( quit == false )
	{
		//And if there's an event to handle
		if( SDL_PollEvent( &event ) )
		{
			//>>> If a key was pressed
			if( event.type == SDL_KEYDOWN )
			{
				//Set the proper message surface
				switch( event.key.keysym.sym )
				{
					case SDLK_UP: message = upMessage; break;
					case SDLK_DOWN: message = downMessage; break;
					case SDLK_LEFT: message = leftMessage; break;
					case SDLK_RIGHT: message = rightMessage; break;
					default : ; //this is just in case it complains
				}
			}
			//>>> If the user wants to X out the window
			else if( event.type == SDL_QUIT )
			{
				//Quit the program
				quit = true;
			}
		}

		//If a message needs to be displayed...
		if( message != NULL )
		{
			//Apply the background to the screen
			apply_surface( 0, 0, background, screen );

			//Apply the message centered on the screen
			apply_surface( ( SCREEN_WIDTH - message->w ) / 2, ( SCREEN_HEIGHT - message->h ) / 2, message, screen );

			//Null the surface pointer
			//Pretty much resets the little message holder after it's done
			message = NULL;
		}

		//Update the screen
		if( SDL_Flip( screen ) == -1 )
		{
			return 1;
		}
	}

	//Clean up
	clean_up();

	return 0;
}


*/