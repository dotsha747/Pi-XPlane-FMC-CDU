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

using namespace std;

// initialize statics
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

	// switch off mouse
	SDL_ShowCursor(SDL_DISABLE);

	// register user events
	SDLUserEventBase = SDL_RegisterEvents(1);

	// Create Window
	SDL_Rect windowRect = { 0, 0, 640, 480 };
	window = SDL_CreateWindow("Server", SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, windowRect.w, windowRect.h, 0);
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

	// Figure out positions of everything on a screen of this resolution, with
	// 24 columns.
	calculateDimensions(24);

	// cache our composited screen as a texture. We have to render the entire screen
	// after each SDL_RenderPresent as on some platforms (e.g. Pi) as there's no guarantee
	// of persistance. So we maintain a texture of the "current" screen at all times.
	// When a line changes, we transfer character textures to this buffer, and then
	// render the entire screen to the display.

	screenBufferTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET, winWidth, winHeight);
	SDL_SetRenderTarget(renderer, screenBufferTexture);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, screenBufferTexture, NULL, NULL);
	SDL_RenderPresent(renderer);

	// prepare our font textures
	SDL_Color bgColor = { 0, 0, 0 };
	SDL_Color fgColor = { 0, 255, 0 };

	string fontfile = "SourceSansPro-Semibold.ttf";
	tallChars = new CharsetTextures(fontfile, window, renderer, cellWidth,
			tallCellHeight, fgColor, bgColor);
	shortChars = new CharsetTextures(fontfile, window, renderer, cellWidth,
			shortCellHeight, fgColor, bgColor);

	cerr << "finished Screen::init" << endl;

}

Screen::~Screen() {

	delete tallChars;
	delete shortChars;
	TTF_Quit();
	SDL_DestroyTexture(screenBufferTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Screen::queueLineUpdate(int line, int col, string text) {

	// push it into the queue
	SDL_Event event;
	SDL_memset(&event, 0, sizeof(event)); /* or SDL_zero(event) */
	event.type = SDLUserEventBase + SDLUserEvent::LineUpdate;
	event.user.data1 = (void *) new LineUpdateTask(line, col, text);
	SDL_PushEvent(&event);

	// cerr << "Pushed into queue  " << line << " " << col << " " << text << endl;
}

void Screen::mainLoop() {

	syslog(LOG_INFO, "in Screen::mainloop");

	Uint32 nextScreenRefresh = SDL_GetTicks();

	bool quit = false;
	while (!quit) {

		SDL_Event event;
		while (SDL_PollEvent(&event)) {

			// process event
			switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				break;
			default:
				if (event.type == SDLUserEventBase + SDLUserEvent::LineUpdate) {
					renderLineAsTexture((LineUpdateTask *) event.user.data1);
					nextScreenRefresh = SDL_GetTicks() + screenRefreshTimerMs;
					// cerr << "Next refresh at " << nextScreenRefresh << endl;
				}
			}

		}

		if (nextScreenRefresh != 0 && SDL_GetTicks() > nextScreenRefresh) {
			// cerr << "Refreshing at " << SDL_GetTicks() << endl;
			CompositeLineTexturesToScreenTexture();
			nextScreenRefresh = 0;
		}

		SDL_Delay(interPollDelayMs);

	}

	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();

	syslog(LOG_INFO, "Finished Screen::mainloop");

}

void Screen::calculateDimensions(int numCols, bool drawGrid) {

	SDL_GetWindowSize(window, &winWidth, &winHeight);

	// we need 25 columns on the screen. Work out the width of each and the
	// left margin.
	cellWidth = winWidth / numCols;
	winLeftBorder = winWidth % numCols / 2;

	// cerr << "Window width: " << winWidth << " height: " << winHeight << endl;
	// cerr << "Column width: " << cellWidth << " leftBorder: " << winLeftBorder << endl;

	// work out the line heights. We have 8 tall lines and 6 short lines. The ratio
	// shortToHeightRatio gives us the ratio of the short lines over the tall lines.
	// Given winHeight as the height of the display, the rowHeight can be defined by
	// the equation:	8h + (shortToHeightRatio  * 6h) = winHeight.
	// Solved for h (https://goo.gl/NxJ08w), this gives us:
	// 		h = winHeight / (2 * ((3 * shortToHeightRatio)  + 4))

	double shortToHeightRatio = 0.8;
	tallCellHeight = winHeight / (2 * ((3 * shortToHeightRatio) + 4));
	shortCellHeight = shortToHeightRatio * tallCellHeight;
	winTopBorder = (winHeight - (8 * tallCellHeight + 6 * shortCellHeight)) / 2;

	// short fonts are displayed on the same baseline as tall fonts. This is the
	// vertical offset to add to shortfont's Y position.
	shortCharVertOffset = ((tallCellHeight - shortCellHeight) / 2) - 1;

	//cerr << "TallRowHeight: " << tallCellHeight << " ShortRowHeight:" << shortCellHeight
	//		<< " winTopBorder: " << winTopBorder<< " shortCharVertOffset: "
	//		<< shortCharVertOffset << endl;

	// work out the Y position of each row and cache it.
	linePos[0] = winTopBorder;
	for (int i = 1; i < 13; i += 2) {
		linePos[i] = linePos[i - 1] + tallCellHeight;
		linePos[i + 1] = linePos[i] + shortCellHeight;
	};
	linePos[13] = linePos[12] + tallCellHeight;
	linePos[14] = linePos[13] + tallCellHeight;

	if (drawGrid) {

		// Set color of renderer to green
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

		// left border = 8

		for (int x = winLeftBorder; x <= winWidth; x += cellWidth) {
			SDL_RenderDrawLine(renderer, x, winTopBorder, x,
					linePos[13] + tallCellHeight);
		}

		for (int y = 0; y <= 14; y++) {
			SDL_RenderDrawLine(renderer, winLeftBorder, linePos[y],
					winLeftBorder + (numCols * cellWidth), linePos[y]);
		}
	}
}



/* @brief render a line to the screen buffer.
 *
 * Different FMCs have different ways of specifiying when small fonts
 * appear on large lines. The code here depends on '{' to indicate
 * switching to a small font, and '}' to indicate switching to a
 * large font, since these characters aren't used in the FMC. It's up
 * to the FMC drivers to translate the specific method the FMC uses
 * into this format.
 *
 */

void Screen::renderLineAsTexture(LineUpdateTask * lineUpdateTask) {

	int line = lineUpdateTask->getLine();
	int col = lineUpdateTask->getCol();

	if (line > 14) {
		ostringstream buf;
		buf << "Can't draw on line " << line << ", as > 14";
		throw runtime_error(buf.str().c_str());
	}

	// special handling for x737 Legs Page
	if (line == 0) {
		isx737FMCLegsPage = lineUpdateTask->getText().find(" LEGS ")
				!= string::npos;
	}
	int baseY = linePos[line];

	// figure out big or small

	CharsetTextures * charset;
	int lineHeight;

	if (line == 0 || line == 2 || line == 4 || line == 6 || line == 8
			|| line == 10 || line == 12 || line == 13) {
		charset = tallChars;
		lineHeight = tallCellHeight;
	} else {
		charset = shortChars;
		lineHeight = shortCellHeight;
	}

	// point renderer to our screenBufferTexture
	SDL_SetRenderTarget(renderer, screenBufferTexture);

	// clear the entire line
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // black
	SDL_Rect lineRect = { 0, baseY, winWidth, lineHeight };
	SDL_RenderFillRect(renderer, &lineRect);

	// for each char, render text at column on this line
	// bool flippedCharset = false;
	int vertOffset = 0;
	for (char c : lineUpdateTask->getText()) {

		if (c == '{') {
			charset = shortChars;
			lineHeight = shortCellHeight;
			vertOffset = shortCharVertOffset;
			continue;
		}

		else if (c == '}') {
			charset = tallChars;
			lineHeight = tallCellHeight;
			vertOffset = 0;
			continue;
		}

		/*

		// for legs page in x737FMC, speed/alt should short characters
		// by default.
		if (isx737FMCLegsPage && col == 12 && line > 0 && line < 12
				&& (line % 2 == 0)) {
			charset = shortChars;
			lineHeight = shortCellHeight;
			vertOffset = shortCharVertOffset;
		}

		// The exception to the above is when something is surrounded in
		// $, in which case that becomes Tall. We just treat $ as a toggle.

		if (c == '$' && isx737FMCLegsPage && col > 12) {
			if (flippedCharset) {
				charset = shortChars;
				lineHeight = shortCellHeight;
				// push the short chars so they sit on the same base as the tall chars
				vertOffset = shortCharVertOffset;
				cerr << "HEERE!!!" << endl;
			} else {
				charset = tallChars;
				lineHeight = tallCellHeight;
				vertOffset = 0;
			};
			flippedCharset = !flippedCharset;
			continue;
		}

		*/


		int baseX = winLeftBorder + (cellWidth * col);

		charset->renderCharAt(renderer, c, baseX, baseY + vertOffset);
		col++;
	}

	// done updating screenBufferTexture
}

void Screen::CompositeLineTexturesToScreenTexture() {

	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, screenBufferTexture, NULL, NULL);
	SDL_RenderPresent(renderer);

}

void Screen::clear() {

	for (int line = 0; line < 13; line++)
		queueLineUpdate(line, 0, "");
}
