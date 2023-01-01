/*
 This file is part of Pi-XPlane-FMC-CDU
 A Raspberry Pi-based External FMC for XPlane

 Copyright (C) 2017-2018 shahada abubakar
 <shahada@abubakar.net>

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.

 */

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <sys/stat.h>
#include <thread>

#include "FMCManager.h"
#include "Screen.h"

using namespace std;

// initialize statics
Screen * Screen::instance = NULL;
int Screen::winWidth = 640;
int Screen::winHeight = 480;

// Constructor

Screen::Screen() {

	syslog(LOG_INFO, "Screen is initializing SDL");

	// initialize SDL2
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		ostringstream buf;
		buf << "SDL_Init Error: " << SDL_GetError();
		SDL_Quit();
		throw runtime_error(buf.str());
	}

	// Initialize SDL2_TTF
	if (TTF_Init() < 0) {
		ostringstream buf;
		buf << "SDL2_TTF could not initialize! SDL_Error: " << SDL_GetError();
		SDL_Quit();
		throw runtime_error(buf.str());
	}

	// open a window
	SDL_Window * window = SDL_CreateWindow("Pi-XPlane-FMC-CDU", 0, 0, winWidth,
			winHeight, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		ostringstream buf;
		buf << "SDL_CreateWindow Error: " << SDL_GetError();
		SDL_Quit();
		throw runtime_error(buf.str());
	}

	// Create a renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr) {
		ostringstream buf;
		buf << "SDL_CreateRender Error: " << SDL_GetError();
		SDL_DestroyWindow(window);
		SDL_Quit();
		throw runtime_error(buf.str());
	}

	// Set size of renderer to the same as window
	SDL_RenderSetLogicalSize(renderer, winWidth, winHeight);

	// switch off mouse
	SDL_ShowCursor(SDL_DISABLE);

	// register user events
	SDLUserEventBase = SDL_RegisterEvents(4);

	// create a texture where we can composite our screen. This can then be
	// blitted to the display.
	screenBufferTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET, winWidth, winHeight);

	// set it to black initially.
	SDL_SetRenderTarget(renderer, screenBufferTexture);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	transferBufferToDisplay();

}

// Destructor

Screen::~Screen() {

	syslog(LOG_INFO, "In Screen destructor()");

	// wait for main thread to stop
	if (isRunning) {
		stopRequested = true;
		// wait up to 5 seconds
		for (int ctr = 0; ctr < 5; ctr++) {
			if (!isRunning) {
				break;
			}
			syslog(LOG_INFO, "Waiting for Screen::mainLoop() to stop");
			sleep(1);

		}
	}

	if (tallChars != NULL) {
		SDL_DestroyTexture(tallChars);
	}

	if (shortChars != NULL) {
		SDL_DestroyTexture(shortChars);
	}

	if (screenBufferTexture != NULL) {
		SDL_DestroyTexture(screenBufferTexture);
	}

	TTF_Quit();

	if (renderer != NULL) {
		SDL_DestroyRenderer(renderer);
	}

	if (window != NULL) {
		SDL_DestroyWindow(window);
	}

	SDL_Quit();

}

Screen * Screen::getInstance() {
	if (!instance) {
		syslog(LOG_INFO, "Instantiating Screen");
		instance = new Screen();
	}
	return instance;
}

void Screen::initialize(int cols) {

	// queue handler is responsible to free this
	ScreenEvent * screenEvent = new ScreenEvent();

	screenEvent->col = cols;

	SDL_Event sdlEvent;
	SDL_memset(&sdlEvent, 0, sizeof(SDL_Event));
	sdlEvent.type = SDLUserEventBase + ScreenEventType::init;
	sdlEvent.user.data1 = (void *) screenEvent;

	SDL_PushEvent(&sdlEvent);

}

void Screen::clearScreen() {

	SDL_Event sdlEvent;
	SDL_memset(&sdlEvent, 0, sizeof(SDL_Event));
	sdlEvent.type = SDLUserEventBase + ScreenEventType::clear;
	sdlEvent.user.data1 = NULL; // clear requires no data

	SDL_PushEvent(&sdlEvent);
}

void Screen::drawLine(int col, int row, std::string text, bool spaceErases,
		char color, bool smallInBig) {

	// queue handler is responsible to free this
	ScreenEvent * screenEvent = new ScreenEvent();

	screenEvent->col = col;
	screenEvent->line = row;
	screenEvent->text = text;
	screenEvent->spaceErases = spaceErases;
	screenEvent->color = color;
	screenEvent->smallInBig = smallInBig;

	SDL_Event sdlEvent;
	SDL_memset(&sdlEvent, 0, sizeof(SDL_Event));
	sdlEvent.type = SDLUserEventBase + ScreenEventType::LineUpdate;
	sdlEvent.user.data1 = (void *) screenEvent;

	SDL_PushEvent(&sdlEvent);

}

void Screen::quitLoop() {

	SDL_Event sdlEvent;
	SDL_memset(&sdlEvent, 0, sizeof(SDL_Event));
	sdlEvent.type = SDLUserEventBase + ScreenEventType::quit;
	sdlEvent.user.data1 = NULL; // quitrequires no data

	SDL_PushEvent(&sdlEvent);

}

void Screen::mainLoop() {

	syslog(LOG_INFO, "in Screen::mainloop");

	bool isRunning = true;
	bool stopRequested = false;
	time_t lastTick = time(NULL);

	while (!stopRequested) {

		SDL_Event event;
		if (SDL_PollEvent(&event)) {

			// process event
			if (event.type == SDL_QUIT) {
				stopRequested = true;
			}

			else if (event.type == SDL_KEYDOWN) {
				// do nothing?
			}

			else if (event.type == SDLUserEventBase + ScreenEventType::init) {
				doInit(&event);
			}

			else if (event.type == SDLUserEventBase + ScreenEventType::clear) {
				doClear();
			}

			else if (event.type
					== SDLUserEventBase + ScreenEventType::LineUpdate) {
				doDrawLine(&event);
			}

			else if (event.type == SDLUserEventBase + ScreenEventType::quit) {
				stopRequested = true;
			} else {
				// do nothing
			}

			continue;

		}

		SDL_Delay(interPollDelayMs);

		// tick on new second
		time_t nowTime = time(NULL);
		if (nowTime > lastTick) {
			lastTick = nowTime;
			FMCManager::getInstance()->tick();
		}

	}

	syslog(LOG_INFO, "Finished Screen::mainloop");

}

void Screen::doInit(SDL_Event * event) {

	ScreenEvent * screenEvent = (ScreenEvent *) event->user.data1;

	// if screen is being reinitialized with same number of columns,
	// we don't have to re-calculate metrics.

	if (numCols != screenEvent->col) {

		numCols = screenEvent->col;

		// calculate metrics

		syslog(LOG_INFO, "Screen::doInit (numCols=%d)", numCols);

		// Work out the width of each cell and the left margin.
		cellWidth = winWidth / numCols;
		winLeftBorder = winWidth % numCols / 2;

		// work out the line heights. We have 8 tall lines and 6 short lines. The ratio
		// shortToHeightRatio gives us the ratio of the short lines over the tall lines.
		// Given winHeight as the height of the display, the rowHeight can be defined by
		// the equation:	8h + (shortToHeightRatio  * 6h) = winHeight.
		// Solved for h (https://goo.gl/NxJ08w), this gives us:
		// 		h = winHeight / (2 * ((3 * shortToHeightRatio)  + 4))

		double shortToHeightRatio = 0.7;
		tallCellHeight = winHeight / (2 * ((3 * shortToHeightRatio) + 4));
		shortCellHeight = shortToHeightRatio * tallCellHeight;
		winTopBorder = (winHeight - (8 * tallCellHeight + 6 * shortCellHeight))
				/ 2;

		// short fonts are displayed on the same baseline as tall fonts. This is the
		// vertical offset to add to shortfont's Y position.
		shortCharVertOffset = ((tallCellHeight - shortCellHeight) / 2) - 1;

		ostringstream buf;
		buf << "Screen has calculated metrics: cellWidth=" << cellWidth
				<< " winLeftBorder=" << winLeftBorder << " tallCellHeight="
				<< tallCellHeight << " shortCellHeight=" << shortCellHeight
				<< " winTopBorder" << winTopBorder << " shortCharVertOffset="
				<< shortCharVertOffset;

		// work out the Y position of each row and cache it.
		linePos[0] = winTopBorder;
		for (int i = 1; i < 13; i += 2) {
			linePos[i] = linePos[i - 1] + tallCellHeight;
			linePos[i + 1] = linePos[i] + shortCellHeight;
		};
		linePos[13] = linePos[12] + tallCellHeight;
		linePos[14] = linePos[13] + tallCellHeight;

		for (int i = 0; i < 13; i++) {
			buf << " line" << i << "=" << linePos[i];
		}

		syslog(LOG_INFO, "%s", buf.str().c_str());

		// search for our font file. We prefer to use LiberationSans-Regular. Try the default
		// location in raspbian first, falling back to the Ubuntu location.
		struct stat buffer;
		string fontPath;
		fontPath = "/usr/local/fonts/consola.ttf";
		if (stat(fontPath.c_str(), &buffer) != 0) {
			fontPath = "NimbusSanL-Regu.ttf";
			if (stat(fontPath.c_str(), &buffer) != 0) {
				fontPath =
						"/usr/share/fonts/truetype/liberation2/LiberationSans-Regular.ttf";
				if (stat(fontPath.c_str(), &buffer) != 0) {
					fontPath =
							"/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf";
					if (stat(fontPath.c_str(), &buffer) != 0) {
						throw runtime_error(
								"Unable to find font LiberationSans-Regular.ttf");
					}
				}
			}
		}
		syslog (LOG_INFO, "using font \"%s\"", fontPath.c_str());

		// generate font textures
		if (tallChars != NULL) {
			SDL_DestroyTexture(tallChars);
		}
		tallChars = generateCharacterTexture(fontPath, cellWidth,
				tallCellHeight, renderer);

		if (shortChars != NULL) {
			SDL_DestroyTexture(shortChars);
		}
		shortChars = generateCharacterTexture(fontPath, cellWidth,
				shortCellHeight, renderer);

	} else {
		syslog(LOG_INFO, "Skipping re-calculating metrics");
	}

	// clear the screen
	doClear();

	syslog(LOG_INFO, "Screen::doInit completed");

	delete screenEvent;

}

void Screen::doClear() {

	SDL_SetRenderTarget(renderer, screenBufferTexture);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	transferBufferToDisplay();

}

void Screen::doDrawLine(SDL_Event * event) {

	ScreenEvent * screenEvent = (ScreenEvent *) event->user.data1;

	int col = screenEvent->col;
	int line = screenEvent->line;
	string text = screenEvent->text;
	bool spaceErases = screenEvent->spaceErases;
	char color = screenEvent->color;
	bool smallInBig = screenEvent->smallInBig;

	syslog(LOG_INFO, "Screen::doLineUpdate (col=%d, line=%d, text=\"%s\" %c %c",
			col, line, text.c_str(), (spaceErases ? 'Y' : 'N'), color);

	if (line > 13) {
		ostringstream buf;
		buf << "Can't draw on line " << line << ", as > 14";
		throw runtime_error(buf.str().c_str());
	}

	// By default, the size of the font depends on which line we're on
	bool isTall;
	int cellHeight;
	int vertOffset = 0;
	if (line == 0 || line == 2 || line == 4 || line == 6 || line == 8
			|| line == 10 || line == 12 || line == 13) {
		isTall = true;
		cellHeight = tallCellHeight;
		vertOffset = 0;
	} else {
		isTall = false;
		cellHeight = shortCellHeight;
		vertOffset = shortCharVertOffset;
	}

	int baseY = linePos[line];

	// point renderer to our screenBufferTexture
	SDL_SetRenderTarget(renderer, screenBufferTexture);

	// iterate through each character
	for (char c : text) {

		// where our cell's X position is
		int baseX = winLeftBorder + (cellWidth * col);

		// '{' means switch to small font. Alternative to zibo-style overlays.
		if (c == '{') {
			isTall = false;
			cellHeight = shortCellHeight;
			vertOffset = shortCharVertOffset;
			continue;
		}

		// '}' means switch to large font.  Alternative to zibo-style overlays.
		else if (c == '}') {
			isTall = true;
			cellHeight = tallCellHeight;
			vertOffset = 0;
			continue;
		}

		// ' ' blanks the cell, depending on spaceErases
		else if (c == ' ') {
			if (spaceErases) {
				SDL_Rect spaceRect = { baseX, baseY, cellWidth, cellHeight };
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_RenderFillRect(renderer, &spaceRect);
			};
			col++;
			continue;
		}

		else if ( c == '`') {
			c = '=';
		}

		// render the character to the buffer texture
		if (islower(c)) {
			c = toupper(c);
		}

		// figure the area of the character in font texture
		SDL_Rect srcrect = { (c - 32) * cellWidth, 0, cellWidth, cellHeight };

		// the destination in the buffer
		SDL_Rect dstrect = { baseX, baseY + vertOffset, cellWidth, cellHeight };

		int r = 255;
		int g = 255;
		int b = 255;

		if (color == 'M') {
			g = 0;
		}

		if (color == 'G') {
			r =0;
			b=0;
		}

		// bitblt
		if (isTall) {
			SDL_SetTextureColorMod (tallChars, r, g, b);
			SDL_RenderCopy(renderer, tallChars, &srcrect, &dstrect);
		} else {
			SDL_SetTextureColorMod (tallChars, r, g, b);
			SDL_RenderCopy(renderer, shortChars, &srcrect, &dstrect);
		}
		col++;
	}

	transferBufferToDisplay();

	delete screenEvent;
}

void Screen::transferBufferToDisplay() {
	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, screenBufferTexture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

SDL_Texture * Screen::generateCharacterTexture(std::string fontPath,
		int maxWidth, int maxHeight, SDL_Renderer * renderer) {

	// create a big surface to hold all the characters we may ever need,
	// making it the same pixelformat as the screen.
	// Initalize Color Masks.

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	int redMask = 0xff000000;
	int greenMask = 0x00ff0000;
	int blueMask = 0x0000ff00;
	int alphaMask = 0x000000ff;
#else
	int redMask = 0x000000ff;
	int greenMask = 0x0000ff00;
	int blueMask = 0x00ff0000;
	int alphaMask = 0xff000000;
#endif
	int w = (96 - 32) * cellWidth;

	SDL_Surface * allCharSurface = SDL_CreateRGBSurface(0, w, maxHeight, 32,
			redMask, greenMask, blueMask, alphaMask);
	if (allCharSurface == NULL) {
		cerr << "SDL_CreateRGBSurface failed for allCharSurface: "
				<< SDL_GetError() << endl;
		exit(1);
	}

	syslog(LOG_INFO, "Creating allCharacter surface of width=%d height=%d", w,
			maxHeight);

	// paint the whole surface black
	SDL_FillRect(allCharSurface, NULL,
			SDL_MapRGB(allCharSurface->format, 0, 0, 0));

	// get a font of the appropriate size to fit our cell width and height
	TTF_Font * font = getFont(fontPath, maxWidth, maxHeight, renderer);
	SDL_Color white = { 255, 255, 255 };
	SDL_Color black = { 0, 0, 0 };

	// now let's render printable characters
	for (char c = 32; c < 96; c++) {

		// make a Unicode string
		Uint16 uc[2];
		if (c == '=') {
			// interpret equals as degree symbol
			uc[0] = 0x00b0;
		}  else {
			uc[0] = c;
		};
		uc[1] = 0;

		// render that character to it's own surface
		SDL_Surface* shaded = TTF_RenderUNICODE_Shaded(font,
				(const Uint16 *) uc, white, black);

		// work out horizontal offset to center this char within a cell
		int hOffset = (cellWidth - shaded->w) / 2;

		// copy the character surface into allCharSurface
		SDL_SetSurfaceBlendMode(shaded, SDL_BLENDMODE_NONE);
		SDL_Rect dstrect = { ((c - 32) * cellWidth) + hOffset, 0, shaded->w,
				shaded->h };
		SDL_BlitSurface(shaded, NULL, allCharSurface, &dstrect);

		cerr << "Char " << (int) c << " [" << c << "] rendered width is w:"
				<< shaded->w << " h:" << shaded->h << " at allCharSet ["
				<< dstrect.x << ", " << dstrect.y << ", " << dstrect.w << ", "
				<< dstrect.h << "]" << endl;

		// release the shadedSurface
		SDL_FreeSurface(shaded);
	}

	// release the font
	TTF_CloseFont(font);

	ostringstream buf;
	buf << "allChars" << maxHeight << ".bmp";
	SDL_SaveBMP(allCharSurface, buf.str().c_str());

	// convert allCharSurface to allCharTexture
	SDL_Texture * allCharTexture = SDL_CreateTextureFromSurface(renderer,
			allCharSurface);
	SDL_FreeSurface(allCharSurface);

	return allCharTexture;

}

TTF_Font * Screen::getFont(std::string fontPath, int cellWidth, int cellHeight,
		SDL_Renderer * renderer) {

	TTF_Font * font = NULL;
	int size = cellHeight;
	int dir = 0;
	int testHeight;

	syslog(LOG_INFO, "Trying to generate font for width=%d height=%d",
			cellWidth, cellHeight);

	while (true) {

		if (font != NULL) {
			TTF_CloseFont(font);
		}

		font = TTF_OpenFont(fontPath.c_str(), size);
		//TTF_SetFontStyle (font, TTF_STYLE_BOLD);
		if (font == NULL) {
			ostringstream buf;
			buf << "SDL2_TTF could not load font from \"" << fontPath
					<< "\" size " << size << "pt, SDL_Error: "
					<< SDL_GetError();
			throw runtime_error(buf.str());
		}

		// Render the character "W" and get the width.
		SDL_Color white = { 255, 255, 255 };
		SDL_Color black = { 0, 0, 0 };
		SDL_Surface * surface = TTF_RenderText_Shaded(font, "W", white, black);
		SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
		int texW = 0;
		int texH = 0;
		SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
		SDL_FreeSurface(surface);
		SDL_DestroyTexture(texture);
		testHeight = TTF_FontHeight(font);

		cerr << "Font size " << size << "pt has width:" << texW
				<< " and height " << testHeight << endl;

		if ((testHeight < cellHeight && texW < cellWidth) && dir != -1) {
			size++;
			dir = 1;
		} else if ((testHeight > cellHeight || texW > cellWidth) && dir != 1) {
			size--;
			dir = -1;
		} else {
			break;
		}
	}

	ostringstream buf;
	buf << "Using font \"" << fontPath << "\" size " << size << "pt, height = "
			<< testHeight << " to fill row of height " << cellHeight << endl;
	syslog(LOG_INFO, "%s", buf.str().c_str());

	return font;
}

/*
 // ==== OLD CODE BELOW ====

 void Screen::initzoo() {

 cerr << "In Screen::init" << endl;

 // init attributes
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
 // SDL_ShowCursor(SDL_DISABLE);

 // register user events
 SDLUserEventBase = SDL_RegisterEvents(1);

 // Create Window
 SDL_Rect windowRect = { 0, 0, 640, 480 };
 window = SDL_CreateWindow("Server",
 SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowRect.w,
 windowRect.h, 0 | SDL_WINDOW_SHOWN);
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

 // render some text directly to the screenBufferTexture
 cerr << "Boo" << endl;

 // search for our font file. We prefer to use liberationSans-regular. Try the default
 // location in raspbian first, falling back to the Ubuntu location.
 struct stat buffer;
 string fontLocation =
 "/usr/share/fonts/truetype/liberation2/LiberationSans-Regular.ttf";
 if (stat(fontLocation.c_str(), &buffer) != 0) {
 fontLocation =
 "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf";
 if (stat(fontLocation.c_str(), &buffer) != 0) {
 cerr << "Unable to locate font." << endl;
 exit(1);
 }
 }

 TTF_Font * font = TTF_OpenFont(fontLocation.c_str(), 25);
 if (!font) {
 cerr << "Failed to open font" << endl;
 exit(1);
 }

 SDL_Color color = { 255, 255, 255 };
 SDL_Color bgcolor = { 0, 0, 0 };
 SDL_Surface * surface = TTF_RenderText_Shaded(font,
 "Welcome to Pi-XPlane-FMC-CDU", color, bgcolor);
 SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);

 int texW = 0;
 int texH = 0;
 SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
 SDL_Rect dstrect = { 0, 0, texW, texH };

 SDL_RenderCopy(renderer, texture, NULL, &dstrect);

 // draw a line
 SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
 int w, h;
 SDL_GetWindowSize(window, &w, &h);
 SDL_RenderDrawLine(renderer, 0, 0, w, h);

 SDL_SetRenderTarget(renderer, NULL);
 SDL_RenderCopy(renderer, screenBufferTexture, NULL, NULL);
 SDL_RenderPresent(renderer);

 // prepare our font textures
 SDL_Color bgColor = { 0, 0, 0 };
 SDL_Color fgColor = { 255, 255, 255 };

 tallChars = new CharsetTextures(fontLocation, window, renderer, cellWidth,
 tallCellHeight, fgColor, bgColor);
 shortChars = new CharsetTextures(fontLocation, window, renderer, cellWidth,
 shortCellHeight, fgColor, bgColor);

 cerr << "finished Screen::init" << endl;

 SDL_Texture * t = tallChars->getAllCharTexture();
 SDL_Rect d = { 0, 0, 100, 14 };
 SDL_RenderCopy(renderer, t, NULL, &d);

 SDL_SetRenderTarget(renderer, NULL);
 SDL_RenderCopy(renderer, screenBufferTexture, NULL, NULL);
 SDL_RenderPresent(renderer);

 sleep(3);

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
 */
