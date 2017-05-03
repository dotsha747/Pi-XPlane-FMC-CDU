/*
 * Screen.cpp
 *
 *  Created on: Apr 10, 2017
 *      Author: shahada
 */

#include <stdio.h>
#include <iostream>
#include <sstream>

#include "Screen.h"

/** TODO:
 * 	- Screen should have a char buffer for the entire display.
 * 	- Screen should export methods that allow other threads to update the display.
 * 	- These methods should be mutex'd by Screen.
 * 	- These methods should post an SDL Event to wake the Screen SDL Loop
 * 	- The main loop should grab the user event, and use that to render the changes.
 * methods
 */

using namespace std;

Screen * Screen::instance = NULL;

void Screen::init() {

	cerr << "In Screen::init" << endl;

	isx737FMCLegsPage = false;

	//Initialize SDL
	if (SDL_Init( SDL_INIT_EVERYTHING) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		exit(1);
	};

	// Initialize SDL2_TTF
	if (TTF_Init() < 0) {
		cerr << "SDL2_TTF could not initialize! SDL_Error: " << SDL_GetError()
				<< endl;
		exit(1);
	}

	// register user events
	SDLUserEventBase = SDL_RegisterEvents(1);

	// Create Window
	SDL_Rect windowRect = { 0, 0, 640, 480 };
	window = SDL_CreateWindow("Server", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			windowRect.w, windowRect.h, 0);
	if (window == nullptr) {
		cerr << "Failed to create window : " << SDL_GetError() << endl;
		exit(-1);
	}

	// Create Renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr) {
		cerr << "Failed to create renderer : " << SDL_GetError();
		exit(-1);
	}

	// Set size of renderer to the same as window
	SDL_RenderSetLogicalSize(renderer, windowRect.w, windowRect.h);

	// Set fill colour
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	// Clear the window
	SDL_RenderClear(renderer);

	calculateDimensions (false);

	// prepare our font textures
	SDL_Color bgColor = { 0, 0, 0};
	SDL_Color fgColor = { 0, 255, 0};

	string fontfile = "NimbusSanL-Regu.ttf";
	tallChars = new CharsetTextures(fontfile, window, renderer, cellWidth, tallCellHeight, fgColor, bgColor);
	shortChars = new CharsetTextures(fontfile, window, renderer, cellWidth, shortCellHeight, fgColor, bgColor);

	SDL_RenderPresent (renderer);

	cerr << "finished Screen::init" << endl;

}




void Screen::mainLoop () {

	syslog (LOG_INFO, "in Screen::mainloop");

	bool quit = false;
	while (!quit) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				break;
			default:
				if (event.type == SDLUserEventBase + SDLUserEvent::LineUpdate) {
					LineUpdateTask * lineUpdateTask =
							(LineUpdateTask *) event.user.data1;
					renderTextLine(lineUpdateTask->getLine(),
							lineUpdateTask->getCol(),
							lineUpdateTask->getText());
					delete lineUpdateTask;
					SDL_RenderPresent(renderer);
				}
			}
		}

	}

	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();

	syslog (LOG_INFO, "Finished Screen::mainloop");

}

Screen::~Screen() {
	// TODO Auto-generated destructor stub
}






void Screen::renderTextLine(int line, int col, std::string text) {

	if (line > 14) {
		ostringstream buf;
		buf << "Can't draw on line " << line << ", as > 14";
		throw runtime_error(buf.str().c_str());
	}

	// special handling for x737 Legs Page
	if (line == 0) {
		isx737FMCLegsPage = text.find(" LEGS ") != string::npos;
	}
	cerr << "isx737FMCLegsPage" << isx737FMCLegsPage << endl;
	int baseY = linePos[line] ;

	// figure out big or small

	CharsetTextures * charset;
	int lineHeight;
	int vertOffset = 0;

	if (line == 0 || line == 2 || line == 4 || line == 6 || line == 8
			|| line == 10 || line == 12 || line == 13) {
		charset = tallChars;
		lineHeight = tallCellHeight;
	} else {
		charset = shortChars;
		lineHeight = shortCellHeight;
	}

	// clear the entire line
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // black
	SDL_Rect lineRect = { 0, baseY, 640, lineHeight };
	SDL_RenderFillRect(renderer, &lineRect);


	// for each char, render text at column
	bool flippedCharset = false;
	for (char c : text) {

		// for legs page in x737FMC, speed/alt should short characters
		// by default.
		if (isx737FMCLegsPage && col == 12 && line > 0 && line < 12) {
			charset = shortChars;
			lineHeight = shortCellHeight;
		}

		// The exception to the above is when something is surrounded in
		// $, in which case that becomes Tall. We just treat $ as a toggle.

		if (isx737FMCLegsPage && c == '$' && col > 12) {
			if (flippedCharset) {
				charset = shortChars;
				lineHeight = shortCellHeight;
				// push the short chars so they sit on the same base as the tall chars
				vertOffset = shortCharVertOffset;
			} else {
				charset = tallChars;
				lineHeight = tallCellHeight;
				vertOffset = 0;
			};
			flippedCharset = !flippedCharset;
			continue;
		}

		int baseX = winLeftBorder + (cellWidth * col);

		charset->renderCharAt(renderer, c, baseX, baseY + vertOffset);

		col++;
	}



}

void Screen::queueLineUpdate(int line, int col, string text) {

	// push it into the queue
	SDL_Event event;
	SDL_memset(&event, 0, sizeof(event)); /* or SDL_zero(event) */
	event.type = SDLUserEventBase + SDLUserEvent::LineUpdate;
	event.user.data1 = (void *) new LineUpdateTask(line, col, text);
	SDL_PushEvent(&event);

	cerr << "Pushed into queue  " << line << " " << col << " " << text << endl;
}





void Screen::calculateDimensions (bool drawGrid) {

	SDL_GetWindowSize (window, &winWidth, &winHeight);

	// we need 25 columns on the screen. Work out the width of each and the
	// left margin.
	cellWidth =  winWidth / 25;
	winLeftBorder = winWidth % 25 / 2;

	cerr << "Window width: " << winWidth << " height: " << winHeight << endl;
	cerr << "Column width: " << cellWidth << " leftBorder: " << winLeftBorder << endl;

	// work out the line heights. We have 8 tall lines and 6 short lines. The ratio
	// shortToHeightRatio gives us the ratio of the short lines over the tall lines.
	// Given winHeight as the height of the display, the rowHeight can be defined by
	// the equation:	8h + (shortToHeightRatio  * 6h) = winHeight.
	// Solved for h (https://goo.gl/NxJ08w), this gives us:
	// 		h = winHeight / (2 * ((3 * shortToHeightRatio)  + 4))

	double shortToHeightRatio = 0.6;
	tallCellHeight = winHeight / (2 * ((3 * shortToHeightRatio)  + 4));
	shortCellHeight = shortToHeightRatio * tallCellHeight;
	winTopBorder = (winHeight - (8 * tallCellHeight + 6 * shortCellHeight)) / 2;

	// short fonts are displayed on the same baseline as tall fonts. This is the
	// vertical offset to add to shortfont's Y position.
	shortCharVertOffset = tallCellHeight - shortCellHeight-1;

	cerr << "TallRowHeight: " << tallCellHeight << " ShortRowHeight:" << shortCellHeight
			<< " winTopBorder: " << winTopBorder<< " shortCharVertOffset: "
			<< shortCharVertOffset << endl;

	// work out the Y position of each row and cache it.
	linePos[0] = winTopBorder;
	for (int i = 1; i < 13; i+=2) {
		linePos[i] = linePos[i-1]+tallCellHeight;
		linePos[i+1] = linePos[i]+shortCellHeight;
	};
	linePos[13] = linePos[12]+tallCellHeight;
	linePos[14] = linePos[13]+tallCellHeight;

	if (drawGrid) {

		// Set color of renderer to green
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

		// left border = 8

		for (int x = winLeftBorder; x <= winWidth; x += cellWidth) {
			SDL_RenderDrawLine(renderer, x, winTopBorder, x, linePos[13]+tallCellHeight);
		}

		for (int y = 0; y <= 14; y++) {
			SDL_RenderDrawLine (renderer, winLeftBorder, linePos[y], winLeftBorder+(25*cellWidth), linePos[y]);
		}

	}




}
