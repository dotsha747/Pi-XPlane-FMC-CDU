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


#include <iostream>
#include <thread>

#include "KeypadScanner.h"
#include "../../FMCManager.h"

using namespace std;

// initialize statics
KeypadScanner * KeypadScanner::instance = NULL;


KeypadScanner::KeypadScanner () {

	pressedCol = -1;
	pressedRow = -1;


	keyMapping['1'] = {1, 1};  //F1 --> L1
	keyMapping['2'] = {2, 1};  //F2 --> L2
	keyMapping['3'] = {3, 1};  //F3 --> L3
	keyMapping['4'] = {4, 1};  //F4 --> L4
	keyMapping['5'] = {5, 1};  //F5 --> L5
	keyMapping['6'] = {6, 1};  //F6 --> L6
	keyMapping['7'] = {7, 1};  //F7 --> R1
	keyMapping['8'] = {8, 1};  //F8 --> R2
	keyMapping['9'] = {1, 2};  //F9 --> R3
	keyMapping['0'] = {2, 2};  //F10 --> R4
	keyMapping['a'] = {3, 2};  //F11 --> R5
	keyMapping['b'] = {4, 2};  //F12 --> R6
	keyMapping['x'] = {7, 3};  //x --> EXEC

	isRunning = false;

}


KeypadScanner::~KeypadScanner() {

	//stop signal handling here?
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
	char c, tmp; 
	while (!stopRequested) {

		//c=getchar();
		scanf("%c", &c);
		scanf("%c", &tmp);
		if((c >= 48 && c <= 57) || (c == 65 || c == 66 || c == 97 || c == 98 || c == 88 || c == 120)){ 
			
			if ( c == 65){
				c = 97;
			}else if (c == 66){
				c = 98;
			}else if (c == 88){
				c = 120;
			}
			pressedRow = keyMapping[c].first;
			pressedCol = keyMapping[c].second;
			// send key release to FMC, and let it figure it out
			cerr << "KeyPressed row " << pressedRow << " col " <<  pressedCol << endl;
			FMCManager::getInstance()->keyPressEvent((int)(pressedRow), (int)(pressedCol));
		} else {
			cerr << "Unknown char pressed: " << c << " .-" << endl;
		}
	}
	cerr << "Finished KeypadScanner::mainLoop" << endl;
}






