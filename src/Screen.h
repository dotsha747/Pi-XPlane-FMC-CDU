/*
 * Screen.h

 *
 *  Created on: Apr 10, 2017
 *      Author: shahada
 */

#ifndef SCREEN_H_
#define SCREEN_H_

#include <syslog.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <list>
#include <mutex>

#include "CharsetTextures.h"

class Screen {
private:
	Screen() {
	}	 // private so it cannot be called

	Screen(Screen const &) {
	}	 // private so it cannot be called

	Screen & operator=(Screen const &) {
		abort();
	}	 // assignment operator is private






protected:
	static Screen * instance;

	SDL_Window* window;
	SDL_Renderer * renderer;
	CharsetTextures * tallChars;
	CharsetTextures * shortChars;

	// window dimensions
	int winWidth, winHeight;
	int cellWidth, winLeftBorder;
	int tallCellHeight, shortCellHeight, winTopBorder;
	int shortCharVertOffset;
	int linePos[15];


	Uint32 SDLUserEventBase;
	enum SDLUserEvent : int { LineUpdate = 0 };

	bool isx737FMCLegsPage;


	/** @brief Line Update Task
	 *
	 * This stores info that is needed to update the Task. It's pushed into
	 * the SDL event queue as
	 */

	class LineUpdateTask {
	private:
		int line;
		int col;
		std::string text;
	public:
		LineUpdateTask (int line, int col, std::string text) {
			this->line = line;
			this->col = col;
			this->text = text;
		}

		int getCol() const {
			return col;
		}

		int getLine() const {
			return line;
		}

		std::string getText() const {
			return text;
		}

	};

protected:

	void calculateDimensions (bool drawGrid);



public:



	static Screen * getInstance() {
		if (!instance) {
			syslog (LOG_INFO, "Intantiating Screen");
			instance = new Screen();
		}
		return instance;
	}

	void init();
	void mainLoop();


	void drawGrid();
	virtual ~Screen();

	void renderTextLine(int line, int col, std::string text);

	/** @brief queue a line update to the screen
	 *
	 * This can be called from any thread, and queues a screen update
	 * for the main Screen thread.
	 *
	 */

	void queueLineUpdate (int line, int col, std::string text);

};

#endif /* SCREEN_H_ */
