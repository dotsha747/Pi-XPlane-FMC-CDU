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

	isx737FMCLegsPage = false;

	//Initialize SDL
	if (SDL_Init( SDL_INIT_EVERYTHING) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		exit(1);
	};

	// register user events
	SDLUserEventBase = SDL_RegisterEvents(1);

	// Create Window
	SDL_Rect windowRect = { 0, 0, 640, 480 };
	window = SDL_CreateWindow("Server", windowRect.x, windowRect.y,
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

	// Setup Renderer

	// does this disable scaling?
	SDL_SetHint (SDL_HINT_RENDER_SCALE_QUALITY, 0);;

	// Set size of renderer to the same as window
	SDL_RenderSetLogicalSize(renderer, windowRect.w, windowRect.h);

	// Set color of renderer to red
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	// Clear the window (draw this later)
	SDL_RenderClear(renderer);

	// Initialize SDL2_TTF
	if (TTF_Init() < 0) {
		cerr << "SDL2_TTF could not initialize! SDL_Error: " << SDL_GetError()
				<< endl;
		exit(1);
	}

	// prepare our font textures
	tallChars = new CharsetTextures("", 40, renderer);
	shortChars = new CharsetTextures("", 26, renderer);

}

void Screen::run() {

	drawGrid();
	drawLine(5, 0, "Hello");
	drawLine(8, 0, "Welcome");

	SDL_RenderPresent(renderer);

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
					drawLine(lineUpdateTask->getLine(),
							lineUpdateTask->getCol(),
							lineUpdateTask->getText());
					delete lineUpdateTask;
				}
				;

			}
		};
		SDL_RenderPresent(renderer);

		SDL_Delay(100);
	}

	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();

}

Screen::~Screen() {
	// TODO Auto-generated destructor stub
}

void Screen::drawGrid() {

	// Set color of renderer to green
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

	// left border = 8

	for (int x = 8; x <= 640; x += 26) {
		SDL_RenderDrawLine(renderer, x, 0, x, 480);
	}

	// major lines = 40 px, minor lines = 26 px;

	int y = 0;
	int line = 0;
	while (line < 12) {

		y += 40;
		SDL_RenderDrawLine(renderer, 0, y, 640, y);

		cout << y << endl;

		y += 26;
		SDL_RenderDrawLine(renderer, 0, y, 640, y);

		cout << y << endl;

		line += 2;
	}

	y += 40;
	cout << y << endl;
	SDL_RenderDrawLine(renderer, 0, y, 640, y);

	y += 40;
	cout << y << endl;
	SDL_RenderDrawLine(renderer, 0, y, 640, y);

	SDL_RenderPresent(renderer);
}

void Screen::drawLine(int line, int col, std::string text) {

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
	int baseY = linePos[line] + 1;

	// figure out big or small

	CharsetTextures * charset;
	int lineHeight;
	int vertOffset = 0;

	if (line == 0 || line == 2 || line == 4 || line == 6 || line == 8
			|| line == 10 || line == 12 || line == 13) {
		charset = tallChars;
		lineHeight = 40;
	} else {
		charset = shortChars;
		lineHeight = 26;
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
		if (isx737FMCLegsPage && col == 13 && line < 12) {
			cerr << "FLIPPING TO short" << endl;
			charset = shortChars;
			lineHeight = 26;
		}

		// The exception to the above is when something is surrounded in
		// $, in which case that becomes Tall. We just treat $ as a toggle.

		if (isx737FMCLegsPage && c == '$' && col > 12) {
			if (flippedCharset) {
				charset = shortChars;
				lineHeight = 26;
				// push the short chars so they sit on the same base as the tall chars
				vertOffset = 13;
			} else {
				charset = tallChars;
				lineHeight = 40;
				vertOffset = 0;
			};
			flippedCharset = !flippedCharset;
			continue;
		}

		int baseX = 8 + (25 * col);
		SDL_Rect charCell = { baseX, baseY+vertOffset, 25, lineHeight };

		SDL_Texture * charTexture = charset->getCharTexture(c);
		SDL_RenderCopy(renderer, charTexture, nullptr, &charCell);
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
