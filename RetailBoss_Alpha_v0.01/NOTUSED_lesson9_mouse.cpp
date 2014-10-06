/*

//Mouse detections
//http://lazyfoo.net/SDL_tutorials/lesson09/index.php

//Always headers
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

const int CLIP_MOUSEOVER = 0;
const int CLIP_MOUSEOUT = 1;
const int CLIP_MOUSEDOWN = 2;
const int CLIP_MOUSEUP = 3;

//The surfaces (areas of drawable graphical memory)
SDL_Surface* buttonSheet = NULL;
SDL_Surface* screen = NULL;

//Set the event structure
SDL_Event event;

//Set up the sprite sheet for the mouse stuff
SDL_Rect clips[ 4 ];

//----------------------------------------------------
class Button
{
	private:
	//button attributes (position, dimensions)
	SDL_Rect box; 

	//the part of the sheet that will be shown
	SDL_Rect* clip; 

	public:
	//Initialize the variables (takes 4 numbers you give to Button)
	Button( int x, int y, int w, int h );

	//handle the events and set the button's sprite region (handles mouse motion/button stuff)
	void handle_events();

	//show the button on the screen
	void show();
};

void set_clips()
{
	//We're going to take only parts of the sheet here
	clips[ CLIP_MOUSEOVER ].x = 0;
	clips[ CLIP_MOUSEOVER ].y = 0;
	clips[ CLIP_MOUSEOVER ].w = 320;
	clips[ CLIP_MOUSEOVER ].h = 240;

	clips[ CLIP_MOUSEOUT ].x = 320;
	clips[ CLIP_MOUSEOUT ].y = 0;
	clips[ CLIP_MOUSEOUT ].w = 320;
	clips[ CLIP_MOUSEOUT ].h = 240;

	clips[ CLIP_MOUSEDOWN ].x = 0;
	clips[ CLIP_MOUSEDOWN ].y = 240;
	clips[ CLIP_MOUSEDOWN ].w = 320;
	clips[ CLIP_MOUSEDOWN ].h = 240;

	clips[ CLIP_MOUSEUP ].x = 320;
	clips[ CLIP_MOUSEUP ].y = 240;
	clips[ CLIP_MOUSEUP ].w = 320;
	clips[ CLIP_MOUSEUP ].h = 240;
}
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
	SDL_WM_SetCaption( "Spooky testing of mouse stuff!", NULL );

	//If everything initialized alright
	return true;
}

bool load_files()
{
	//Load up the background image
	buttonSheet = load_image( "images/button.png" );

	//If there was a problem oading the background...
	if( buttonSheet == NULL )
	{
		return false;
	}

	return true;
}

void clean_up()
{
	//Freee the surfaces!
	SDL_FreeSurface( buttonSheet );

	//Quit SDL
	SDL_Quit();
}
//----------------------------------------------------

Button::Button( int x, int y, int w, int h )
{
	//Set the button attributes
	box.x = x;
	box.y = y;
	box.w = w;
	box.h = h;

	//set the default sprite
	clip = &clips[ CLIP_MOUSEOUT ];
}

void Button::handle_events()
{
	//the mouse offsets
	int x = 0, y = 0;

	// >>> If the mouse moved
	if( event.type == SDL_MOUSEMOTION )
	{
		//get the mouse offsets
		x = event.motion.x;
		y = event.motion.y;

		//if the mouse is over the button...
		if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
		{
			//set the button sprite
			clip = &clips[ CLIP_MOUSEOVER ];
		}
		//but if it's not over the button...
		else
		{
			//set the button sprite for mouseout
			clip = &clips[ CLIP_MOUSEOUT ];
		}
	}

	// >>> If the mouse button is pressed
	if( event.type == SDL_MOUSEBUTTONDOWN )
	{
		//If the left mouse button is pressed
		if( event.button.button == SDL_BUTTON_LEFT )
		{
			//get the mouse offsets
			x = event.button.x;
			y = event.button.y;

			//if the mouse is over the button while pressed...
			if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
			{
			//set the button sprite
				clip = &clips[ CLIP_MOUSEDOWN ];
			}
		}
	}

	// >>> If the mouse button is let go
	if( event.type == SDL_MOUSEBUTTONUP )
	{
		//If the left mouse button is let go...
		if( event.button.button == SDL_BUTTON_LEFT )
		{
			//get the mouse offsets
			x = event.button.x;
			y = event.button.y;

			//if the mouse is over the button while mouse button is let go
			if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
			{
				//set the button sprite
				clip = &clips[ CLIP_MOUSEUP ];
			}
		}
	}
}

void Button::show()
{
	//show the button
	apply_surface( box.x, box.y, buttonSheet, screen, clip );
}

int main( int argc, char* args[] )
{
	//quit flag
	bool quit = false;

	//initialize 
	if( init() == false )
	{
		return 1;
	}

	//load the files
	if( load_files() == false )
	{
		return 1;
	}

	//Clip the sprite sheet
	set_clips();

	//make the button
	Button myButton( 170, 120, 320, 240 );

	//so while the user hasn't quit...
	while( quit == false )
	{
		//if there are events to handle
		if( SDL_PollEvent( &event ) )
		{
			//handle button events
			myButton.handle_events();

			//if the user wants to X out the window
			if( event.type == SDL_QUIT )
			{
				//quit the program
				quit = true;
			}
		}

		//fill the screen with white background to hide emptiness
		SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );

		//Show the button
		myButton.show();

		//Update the screen
		if( SDL_Flip( screen ) == -1 )
		{
			return 1;
		}
	}

	//clean up
	clean_up();

	return 0;
}

*/