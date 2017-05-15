/*
 * KeypadScanner.cpp
 *
 *  Created on: May 5, 2017
 *      Author: shahada
 */

#include <wiringPi.h>
#include <iostream>
#include <iomanip>
#include <thread>

#include "FMCList.h"
#include "KeypadScanner.h"

using namespace std;

// initialize statics
KeypadScanner * KeypadScanner::instance = NULL;


void KeypadScanner::init() {

	wiringPiSetup ();

	// set columns as outputs, HIGH by default
	for (size_t pin = 0 ; pin < columnPins.size(); pin++) {
		digitalWrite (columnPins[pin], 1);
		pinMode (columnPins[pin], OUTPUT);
	}

	// set rows as inputs
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
		pinMode (pin, INPUT);
		pullUpDnControl (rowPins[pin], PUD_OFF);

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
							// still being pressed, do nothing
						} else {
							// multiple keys being pressed or another key pressed
							// without the earlier being released. Do nothing.
						}
					} else {
						// looks like a new key has been pressed
						pressedRow = nowRow;
						pressedCol = nowCol;
						somethingPressed = true;
						pressEvent((int) (nowRow+1),(int)( nowCol+1));
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
				releaseEvent((int) (pressedRow+1), (int) (pressedCol+1));

			}
		}

	}
	cerr << "Finished KeypadScanner::mainLoop" << endl;
}



/** @brief handle a key press. units begin with 1, not zero to match schematics.
 *
 */

void KeypadScanner::pressEvent (int row, int col) {
	cerr << "Pressed r=" << row << ", c=" << col << endl;

	// send the keypress to FMC, and let it figure it out.
	FMCList::getInstance()->keyPressEvent(row, col);
}



/** @brief handle a key release. units begin with 1, not zero, to match schematics.
 *
 */


void KeypadScanner::releaseEvent (int row, int col) {
	cerr << "Released r=" << row << ", c=" << col << endl;

	// send key release to FMC, and let it figure it out
	FMCList::getInstance()->keyReleaseEvent(row, col);
}
