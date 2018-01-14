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

#include <wiringPi.h>
#include <iostream>
#include <iomanip>
#include <thread>

#include "KeypadScanner.h"
#include "FMCManager.h"

using namespace std;

// initialize statics
KeypadScanner * KeypadScanner::instance = NULL;


KeypadScanner::KeypadScanner () {

	pressedCol = -1;
	pressedRow = -1;

	// set columns as outputs, HIGH by default
	for (size_t pin = 0 ; pin < columnPins.size(); pin++) {
		digitalWrite (columnPins[pin], 1);
		pinMode (columnPins[pin], OUTPUT);
	}

	// set rows as inputs, with Pull-UP enabled.
	for (size_t pin = 0; pin < rowPins.size(); pin++) {
		pinMode (rowPins[pin], INPUT);
		pullUpDnControl (rowPins[pin], PUD_UP);
	}

	isRunning = false;

}


KeypadScanner::~KeypadScanner() {

	// reset the pins to INPUTS before exiting
	for (size_t pin = 0 ; pin < columnPins.size(); pin++) {

		digitalWrite (pin, 1);
		pullUpDnControl (rowPins[pin], PUD_OFF);
		pinMode (pin, INPUT);

	}

}


void KeypadScanner::launchThread() {

	cerr << "In KeypadScanner::launchThread" << endl;
	if (!isRunning) {
		std::thread t(&KeypadScanner::mainLoop, this);
		t.detach();
	}
	cerr << "Finished KeypadScanner::launchThread" << endl;
}


void KeypadScanner::mainLoop() {

	cerr << "In KeypadScanner::mainLoop" << endl;
	isRunning = true;
	stopRequested = false;
	somethingPressed = false;
	// Uint32 pressedTick;

	while (!stopRequested) {

		// iterate through the columns
		bool gotPress = false;
		for (size_t nowCol = 0; nowCol < columnPins.size(); nowCol++) {

			// set the current column to LOW

			digitalWrite (columnPins[nowCol], 0);

			// go through the rows, see who is low (pressed)

			for (size_t nowRow = 0; nowRow < rowPins.size(); nowRow++) {

				// delay a bit for the GPIO state to settle
				delayMicroseconds (5);
				int status = digitalRead (rowPins[nowRow]);

				// something pressed
				if (status == 0) {
					gotPress = true;
					if (somethingPressed == true) {
						if (pressedRow == nowRow && pressedCol == nowCol) {

							/*
							if (nowRow == 1 && nowCol == 7 && (SDL_GetTicks() - pressedTick > 5000)) {
								FMCList::getInstance()->setFMC(0);
							}
							*/

						} else {
							// multiple keys being pressed or another key pressed
							// without the earlier being released. Do nothing.
						}
					} else {
						// looks like a new key has been pressed
						pressedRow = nowRow;
						pressedCol = nowCol;
						somethingPressed = true;
						//pressedTick = SDL_GetTicks();

						// send the keypress to FMC module, and let it figure it out.
						cerr << "Keypress row " << nowRow+1 << " col " << nowCol+1 << endl;
						FMCManager::getInstance()->keyPressEvent((int)(nowRow+1), (int)(nowCol+1));

						// quick and dirty delay for 10ms to debounce
						delayMicroseconds (10000);
					}

				}



			}
			// restore the current column to HIGH
			digitalWrite (columnPins[nowCol], 1);


		}

		if (! gotPress) {
			// nothing pressed this scan. If something was pressed before, consider
			// it released.
			if (somethingPressed) {
				somethingPressed = false;

				// send key release to FMC, and let it figure it out
				cerr << "KeyRelease row " << pressedRow+1 << " col " <<  pressedCol+1 << endl;
				FMCManager::getInstance()->keyReleaseEvent((int)(pressedRow+1), (int)(pressedCol+1));
			}
		}

	}
	cerr << "Finished KeypadScanner::mainLoop" << endl;
}






