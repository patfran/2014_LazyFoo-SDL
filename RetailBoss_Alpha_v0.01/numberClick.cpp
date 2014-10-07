
//
//Testing to see if I can click to increase cash
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

int currentMoney = 0;

SDL_Surface* screen = NULL;
SDL_Surface* background = NULL;
SDL_Surface* button = NULL;
SDL_Surface* preCash = NULL;
SDL_Surface* cash = NULL;

TTF_Font* font = NULL;
SDL_Color textColor = { 0, 0, 0 };

SDL_Event event;

//----------------------------------------------------
class Button
{
	private:
	//button attributes (position, dimensions)
	SDL_Rect box;  

	public:
	//Initialize the variables (takes 4 numbers you give to Button)
	Button( int x, int y, int w, int h );

	//handle the events and set the button's sprite region (handles mouse motion/button stuff)
	void handle_events();

	//show the button on the screen
	void show();
};

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
	SDL_WM_SetCaption( "test test test" , NULL );

	//If everything initialized alright
	return true;
}

bool load_files()
{
	//Load up the background image
	background = load_image( "images/background.bmp" );
	button = load_image( "images/buttonBuy.png" );

	//Open up the font you want to use
	font = TTF_OpenFont( "lazy.ttf", 28 );

	//If there was a problem oading the background...
	if( background == NULL || button == NULL )
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
	SDL_FreeSurface( button );
	SDL_FreeSurface( cash );

	//Don't forget to close the font that was used!
	TTF_CloseFont( font );

	//Quit the SDL_ttf
	TTF_Quit();

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
				currentMoney = currentMoney + 1;
				//printf("the number is now %d", currentMoney);
				
				std::stringstream moneyStream;
				moneyStream << currentMoney;
				printf(moneyStream.str().c_str());
				//cash = TTF_RenderText_Solid( font, moneyStream.str().c_str(), textColor );
			}
		}
	}

	// >>> If the mouse button is pressed and let go
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
				currentMoney = currentMoney + 1;
			}
		}
	}
}

void Button::show()
{
	//show the button
	apply_surface( box.x, box.y, button, screen );
}

//----------------------------------------------------
int main( int argc, char* args[] )
{
	printf("initialized! \n");
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
	//currentMoney (pure data) > moneyStream (turned into a string) > money (which gets shown)
	//std::stringstream moneyStream;
	//moneyStream << currentMoney;

	//Apply the images to the screen!
	apply_surface( 0, 0, background, screen );
	
	//make the button
	Button myButton( 100, 100, 193, 100 );

	//So while the user didn't quit, keep it running
	while( quit == false )
	{
		//And while there are events to handle
		while( SDL_PollEvent( &event ) )
		{
			cash = TTF_RenderText_Solid( font, moneyStream.str().c_str(), textColor );
			myButton.handle_events();

			//If the user wants to close the window
			if( event.type == SDL_QUIT )
			{
				//Quit the program
				quit = true;
			}
		}
		//Show the button
		myButton.show();
		
		//If there was an error rendering the text...
		if( cash == NULL )
		{
			return 1;
		}
		apply_surface( 0, 150, cash, screen );

		//Update the screen
		if( SDL_Flip( screen ) == -1 )
		{
			return 1;
		}
	}
	//Always clean up the memory by free images
	clean_up();
	return 0;
}