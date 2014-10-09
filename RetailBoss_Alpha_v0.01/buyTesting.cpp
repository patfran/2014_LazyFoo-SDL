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

SDL_Surface* myScreen = NULL;
SDL_Surface* myBackground = NULL;

SDL_Surface* myCash = NULL; //the cash text
SDL_Surface* myButton = NULL; //the buy button image
//SDL_Surface* myMessage = NULL; //the message text
//SDL_Surface* myCustomer = NULL; //the customer guy

TTF_Font* font = NULL;
SDL_Color textColor = { 0, 0, 0 };

SDL_Event event;

//----------------------------------------------------
//Don't need to change
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

//Don't need to change
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

//Change window caption
bool init()
{
	//Initialize all SDL subsystems
	if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
	{
		return false;
	}

	//Set up the screen
	myScreen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

	//If there was an error in setting up the screen, return false
	if( myScreen == NULL )
	{
		return false;
	}

	//Initialize the SDL_ttf for text and fonts
	if( TTF_Init() == -1 )
	{
		return false;
	}

	//Set the window title caption thing at the top
	SDL_WM_SetCaption( "buy button testing" , NULL );

	//If everything initialized alright
	return true;
}

//Change images to load
bool load_files() 
{
	//Load up the images
	myBackground = load_image( "images/shopmart.bmp" );
	myButton = load_image( "images/buttonBuy.png" );
	//myCustomer = load_image( "images/foo.jpg" );

	//Open up the font you want to use
	font = TTF_OpenFont( "lazy.ttf", 28 );

	//If there was a problem oading the background...
	if( myBackground == NULL ){
		return false;
	}

	//If there was a problem loading the font...
	if( font == NULL ){
		return false;
	}

	return true;
}

//Change surfaces to clean up
void clean_up() //for every surface, need to change
{
	//Freee the surfaces!
	SDL_FreeSurface( myBackground );
	SDL_FreeSurface( myButton );
	SDL_FreeSurface( myCash );
	//SDL_FreeSurface( myMessage );

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

	if( init() == false){ //Initialize the program
		return 1;
	}
	if( load_files() == false ){ //Load up the files
		return 1;
	}

	//----------------
	Uint32 beforeTime = 0; //Uint32 doesn't allow for negative numbers
	Uint32 currentTime;

	Uint32 cashCounter = 0;

	//So while the user didn't quit, keep it running
	while( quit == false ){

		currentTime = SDL_GetTicks();
		if( currentTime > beforeTime + 1000 ){
			beforeTime = currentTime;
			printf( "report: %d \n" );
		}

		apply_surface( 0, 0, myBackground, myScreen ); //background
		apply_surface( ( SCREEN_WIDTH ) / 2, 200, myCash, myScreen ); //message
		//apply_surface( 100, 100, myButton, myScreen ); //button

		 //display the myCash, cashString to hold number, cashCounter to track number
			if( currentTime == 7000 ){
				printf( " world timer is 5000 \n ");

				cashCounter = cashCounter + 10;
				std::stringstream cashString; //change the timer into a string
				cashString << cashCounter;
				myCash = TTF_RenderText_Solid( font, cashString.str().c_str(), textColor ); //render time surface
				apply_surface( ( SCREEN_WIDTH - myCash->w ) / 2, 50, myCash, myScreen ); //apply time surface
				SDL_FreeSurface( myCash ); //free up the time surface
				currentTime = 0;
			}
		

		while( SDL_PollEvent( &event ) ){ //And while there are events to handle		
			if( event.type == SDL_QUIT ){ //If the user wants to close the window
				quit = true; //Quit the program
			}
		}

		//Update the screen
		if( SDL_Flip( myScreen ) == -1 ){
			return 1;
		}
	}
	//Always clean up the memory by free images
	clean_up();
	return 0;
}