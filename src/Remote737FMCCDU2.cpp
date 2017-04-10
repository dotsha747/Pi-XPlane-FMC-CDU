//============================================================================
// Name        : Remote737FMCCDU2.cpp

// Author      : Shahada Abubakar
// Version     :
// Copyright   : Copyright (c) 2014, NEXTSense Sdn Bhd
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <iostream>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

using namespace std;



int main(int argc, char * argv[]) {


	//Initialize SDL
	if (SDL_Init( SDL_INIT_EVERYTHING) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		exit(1);
	};


	// Create Window
	SDL_Rect windowRect = { 0, 0, 640, 480 };
	SDL_Window* window = SDL_CreateWindow( "Server", windowRect.x, windowRect.y, windowRect.w, windowRect.h, 0 );

	if (window == nullptr) {
		cerr << "Failed to create window : " << SDL_GetError() << endl;
		exit (-1);
	}

	// Create Renderer
	SDL_Renderer * renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
	if ( renderer == nullptr ) {
		cerr << "Failed to create renderer : " << SDL_GetError();
		return false;
	}

	// Setup Renderer

	// Set size of renderer to the same as window
	SDL_RenderSetLogicalSize( renderer, windowRect.w, windowRect.h );

	// Set color of renderer to red
	SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );


	// Initialize SDL2_TTF
	if (TTF_Init() < 0) {
		cerr << "SDL2_TTF could not initialize! SDL_Error: " << SDL_GetError() << endl;
		exit(1);
	}

	// Load Font
	TTF_Font * font = TTF_OpenFont("/home/shahada/.fonts/MONACO.TTF", 32);
	if (font == NULL) {
		cerr << "SDL2_TTF could not load font! SDL_Error: " <<  SDL_GetError() << endl;
		exit(1);
	}

	// Create Text Textures
	SDL_Rect solidRect;
	SDL_Rect blendedRect;
	SDL_Rect shadedRect;
	SDL_Texture* solidTexture;
	SDL_Texture* blendedTexture;
	SDL_Texture* shadedTexture;
	SDL_Color textColor = { 255, 255, 255, 255 }; // white
	SDL_Color backgroundColor = { 0, 0, 0, 255 }; // black

	SDL_Surface* solid = TTF_RenderText_Solid( font, "solid", textColor );
	solidTexture = SDL_CreateTextureFromSurface (renderer, solid);
	SDL_FreeSurface (solid);
	SDL_QueryTexture( solidTexture, NULL, NULL, &solidRect.w, &solidRect.h );
	solidRect.x = 0;
	solidRect.y = 0;

	SDL_Surface* blended = TTF_RenderText_Blended( font, "blended", textColor );
	blendedTexture = SDL_CreateTextureFromSurface (renderer, blended);
	SDL_FreeSurface (blended);
	SDL_QueryTexture( blendedTexture, NULL, NULL, &blendedRect.w, &blendedRect.h );
	blendedRect.x = 0;
	blendedRect.y = solidRect.y + solidRect.h +  20;

	SDL_Surface* shaded = TTF_RenderText_Shaded( font, "shaded", textColor, backgroundColor );
	shadedTexture = SDL_CreateTextureFromSurface (renderer, shaded);
	SDL_FreeSurface (shaded);
	SDL_QueryTexture( shadedTexture , NULL, NULL, &shadedRect.w, &shadedRect.h );
	shadedRect.x = 0;
	shadedRect.y = blendedRect.y + blendedRect.h + 20;

	// Render!

	// Clear the window and make it all red
	SDL_RenderClear (renderer);

	// Render our text objects (like normal)
	SDL_RenderCopy (renderer, solidTexture, nullptr, &solidRect);
	SDL_RenderCopy (renderer, blendedTexture, nullptr, &blendedRect);
	SDL_RenderCopy (renderer, shadedTexture, nullptr, &shadedRect);

	// Render the changes above
	SDL_RenderPresent (renderer);

	std::cout << "Press any key to exit\n";
		std::cin.ignore();


	TTF_CloseFont(font);

	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}



