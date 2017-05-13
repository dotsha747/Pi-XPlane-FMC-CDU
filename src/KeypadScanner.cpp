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

#include "KeypadScanner.h"

using namespace std;

// initialize statics
KeypadScanner * KeypadScanner::instance = NULL;

/** TODO:
 * 	- create variable to track "currentlydown" key
 * 	- report on "currentlydown" changed
 *
 * 	- create variable to track "nobodypressed"
 * 	- currentlydown cannot change unless nobodypressed
 *
 * 	- map currentlydown to dataref
 * 	- subscribe to datarefs
 * 	- send keypress to dataref
 *
 * 	- does dataref stay down if button held down?
 *
 */


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
	while (!stopRequested) {

		// iterate through the columns
		for (size_t nowCol = 0; nowCol < columnPins.size(); nowCol++) {

			// set the current column to LOW
			digitalWrite (columnPins[nowCol], 0);

			// go through the rows, see who is low (pressed)
			for (size_t nowRow = 0; nowRow < rowPins.size(); nowRow++) {

				// delay a bit for the GPIO state to settle
				delayMicroseconds (5);
				int status = digitalRead (rowPins[nowRow]);

				if (status == 0) {
					cerr << "r=" << nowRow+1 << ",c=" << nowCol+1 << endl;
				}



			}
			// restore the current column to HIGH
			digitalWrite (columnPins[nowCol], 1);


		}

	}
	cerr << "Finished KeypadScanner::mainLoop" << endl;
}
