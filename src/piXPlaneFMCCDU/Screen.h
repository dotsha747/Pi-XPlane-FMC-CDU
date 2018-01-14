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

#ifndef PIXPLANEFMCCDU_SRC_SCREEN_H_
#define PIXPLANEFMCCDU_SRC_SCREEN_H_

#include <iostream>
#include <syslog.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <list>
#include <mutex>


/** @brief Manage the FMC CDU display.
 *
 * 	This is singleton class that manages the screen of the FMC CDU.
 *
 * 	It uses SDL2 to render the display.
 *
 * 	The class is used by calling submit* methods, which queue activities
 * 	to be rendered a screen handling thread. This ensures that only one
 * 	thread is making SDL2 calls at a time.
 *
 * 	The class managed a single texture (screenBufferTexture) that holds
 * 	the current display. All drawing is done onto this texture, which
 * 	is then rendered to the display in its entirety.
 *
 * 	The screen uses truetype fonts, however for performance reasons, it
 * 	pre-caches the characters used by the FMC into a texture. Whenever a
 * 	character needs to drawn, a single character is copied to the
 * 	appropriate position on the screenBufferTexture.
 *
 *
 *
 */

class Screen {
private:
	// private so it cannot be called publicly
	Screen();

	// private so it cannot be called publicly
	Screen(Screen const &) {
	}

	// assignment operator is private
	Screen & operator=(Screen const &) {
		abort();
	}

protected:
	static Screen * instance;

	bool isRunning = false;
	bool stopRequested = false;

	/** @brief SDL Window */
	SDL_Window* window = NULL;

	/** @brief SDL renderer */
	SDL_Renderer * renderer = NULL;

	/** @brief current number of columns. Set by user via init() */
	int numCols = 0;

	/** @brief current number of rows. Fixed value 8-) */
	int numRows = 14;

	/** @brief width of a character cell on the screen. Calculated by init(). */
	int cellWidth = 0;

	/** @brief the height of a tall character. Calculated by init(). */
	int tallCellHeight = 0;

	/** @brief the height of a short character. Calculated by init(). */
	int shortCellHeight = 0;

	/** @brief left border of the screen.
	 * 	Calculated by init() based on the remainder after dividing by the number of columns.
	 */
	int winLeftBorder = 0;

	/** @brief top border of the screen.
	 *  Calculated by init() based on the remainder after dividing by the number of rows.
	 */
	int winTopBorder = 0;

	/** @brief vertical offset for short chars.
	 * In some modes, we display short chars in a tall cell. This controls the vertical
	 * offset for the short char within the tall cell.
	 */
	int shortCharVertOffset = 0;

	/** @brief screen offsets for each row. */
	int linePos[15];

	/** @brief a texture holding pre-generated tall characters */
	SDL_Texture * tallChars = NULL;

	/** @brief a texture holding pre-generated short characters */
	SDL_Texture * shortChars = NULL;

	/** @brief ready-composited complete screen texture */
	SDL_Texture * screenBufferTexture = NULL;

	/** @brief How many ms to wait after a line changes before repainting the screen.
	 *
	 * When an entire page changes, we will receive
	 14 separate messages from ExtPlane. To refresh the entire
	 display for each line creates a laggy display. So it's
	 better to wait a bit and get all the lines in before
	 repainting the screen. Setting this too high however will
	 result in a longer response time, which becomes obvious
	 when a single cell changes (like when typing into the
	 scratchpad).
	 */

	Uint32 screenRefreshTimerMs = 10;

	/** @brief how long to delay in between polls of the update queue.
	 *
	 * Too long increases lag, too low increases CPU utilization.
	 */

	Uint32 interPollDelayMs = 1;

	Uint32 SDLUserEventBase;

	/** @brief enum of possible event types
	 *
	 */

	enum ScreenEventType
		: int {
			init = 0, LineUpdate = 1, clear = 2, quit = 4
	};

	/** @brief Event Data
	 *
	 * This stores the data for the different types of events that we can
	 * queue. It's passed along the event queue as the userData.
	 *
	 */

	class ScreenEvent {
	public:
		int col; // used by init and drawLine
		int line;	// used by drawLine
		std::string text; // used by drawLine
		bool spaceErases; // space erases if true, skips if false
		char color; // G=Green
		bool smallInBig; // small font in Large line
	};

	bool isx737FMCLegsPage = false;

	virtual ~Screen();


	/** @brief implement an "init" action
	 *
	 */

	void doInit(SDL_Event * event);

	/** @brief implement a "clear" action
	 *
	 */

	void doClear();

	/** @brief implement a "lineUpdate" action
	 *
	 */

	void doDrawLine(SDL_Event * event);

	/** transfer buffer to display */
	void transferBufferToDisplay ();

	/** @brief generate the texture containing all characters for a given cell width and height */
	SDL_Texture * generateCharacterTexture (std::string fontPath, int cellWidth, int maxHeight, SDL_Renderer * renderer);


	/** @brief generate a font that fits within a cell's width and height */
	TTF_Font * getFont (std::string fontPath, int maxWidth ,int maxHeight, SDL_Renderer * renderer);



	// == old stuff ==
	/*
	void calculateDimensions(int numCols, bool drawGrid = false);

	/** @brief initialize the screen metrics.
	 *
	 *  Because different FMCs have different number of columns (looking at you x737FMC),
	 *  the cell positions and fonts are generated dynamically, based on the number of
	 *  columns and rows desired.
	 *
	 *  You must call initScreen before queuing any screen updates. Initscreen will
	 *  generate the appropriate short and tall fonts, and update the internal
	 *  metrics. The display is cleared to black and any previously pending queued
	 *  updates are flushed.
	 *
	 *  initScreen can be called again at any time to reconfigure the screen with
	 *  different metrics, i.e. when switching FMCs.
	 *
	 *

	void initScreen(int numCols, int numRows);

	void drawGrid();

	/** @brief render a line of text onto the screenBufferTexture
	 *
	 *
	void renderLineAsTexture(LineUpdateTask * lineUpdateTask);

	/** @brief copy the screenBufferTexture to the screen
	 *
	 *
	void CompositeLineTexturesToScreenTexture();

	/** @brief clear the screen
	 *
	 * For local use.
	 *

	virtual void clear();

	*/

public:

	/** @brief window width */
	static int winWidth;

	/** @brief window height */
	static int winHeight;
	/** @brief returns the singleton intance of a Screen. */
	static Screen * getInstance();

	/** @brief queues an init command the the screen handler.
	 *
	 *
	 *
	 * This calculates the screen metrics based on how many columns
	 * are required, and regenerates the font textures.
	 *
	 * It can be called multiple times, to handle situations where
	 * the user switches between FMCs that have a different number
	 * of rows and columns.
	 */

	void initialize(int cols);

	/** @brief queue a screen clear
	 *
	 * This can be called from any thread, and queues a screen clear
	 * event. The implementation of this is faster than drawing
	 * blanks in every cell.
	 */

	void clearScreen();

	/** @brief queue a line update to the screen
	 *
	 * This can be called from any thread, and queues a screen update
	 * for the main Screen thread. This results in the string "text"
	 * being drawn on the screen at the position specified by line/col.
	 *
	 */

	void drawLine(int line, int col, std::string text, bool spaceErases=true, char color='W', bool smallInBig=false);

	/** @brief the main loop of the screen.
	 *
	 *  Runs in a loop waiting for SDL events. In then calls the
	 *  action{event} method to process them.
	 *
	 *  Note: SDL really doesn't like it when we run this in a
	 *  separate thread. So main() has to call this and live
	 *  within it happily ever after.
	 *
	 */
	void mainLoop();


	/** @brief stop the screen's mainloop
	 *
	 *  since the screen's mainLoop is also the main program's
	 *  mainLoop, this will cause the program to exit
	 *  gracefully.
	 */

	void quitLoop ();
};

#endif /* PIXPLANEFMCCDU_SRC_SCREEN_H_ */
