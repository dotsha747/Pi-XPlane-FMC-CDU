/*
 This file is part of Pi-XPlane-FMC-CDU
 A Raspberry Pi-based External FMC for XPlane

 Copyright (C) 2017-2018 shahada abubakar
 <shahada@abubakar.net>

File contributed by Lefteris Kororos <lkororos@gmail.com>

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
#include <unistd.h>

#include "RotaryEncScanner.h"
#include "FMCManager.h"

using namespace std;

// initialize statics
RotaryEncScanner * RotaryEncScanner::instance = NULL;

RotaryEncScanner::RotaryEncScanner () {

	count = 0; 

	// set rows as inputs, with Pull-UP enabled.
	for (size_t pin = 0; pin < pinsAB.size(); pin++) {
		pinMode (pinsAB[pin], INPUT);
		pullUpDnControl (pinsAB[pin], PUD_UP);
	}

	isRunning = false;

}

RotaryEncScanner::~RotaryEncScanner() {

}

void RotaryEncScanner::launchThread() {

	cerr << "In RotaryEncScanner::launchThread" << endl;
	if (!isRunning) {
		std::thread t(&RotaryEncScanner::mainLoop, this);
		t.detach();
	}
	cerr << "Finished RotaryEncScanner::launchThread" << endl;
}

void RotaryEncScanner::mainLoop() {

	cerr << "In RotaryEncScanner::mainLoop" << endl;
    int stateA = digitalRead(pinsAB[0]);
    int stateB = digitalRead(pinsAB[1]);
	int prevStateA, prevStateB, seq, prevSeq, delta, change, dentedCount;
    seq = (stateA ^ stateB) | (stateB << 1);
    prevSeq = seq;
    isRunning = true;
	stopRequested = false;

    dentedCount = 0;
	while (!stopRequested) {
        prevStateA = stateA; 
        prevStateB = stateB; 

        stateA = digitalRead(pinsAB[0]);
        stateB = digitalRead(pinsAB[1]);
        seq = (stateA ^ stateB) | (stateB << 1);

        if(seq != prevSeq){
            //we have movement
            delta = (seq - prevSeq) % 4;
            if((delta == 1) || (delta == -3)){
                //We moved one step clockwise
                count++;
                if((count % 2) == 0){
                    change = 1;
                    dentedCount++;
                    cerr << "Dented Count: " << dentedCount << endl;
                }
            }else if (delta == 2){
                //This should not really happen - unless if spinning clockwise too fast
                cerr << "Moved two steps" << endl;
            }else if((delta == -1) || (delta == 3)){
                //We moved one step anti-clockwise
                count --; 
                if((count % 2) == 0){
                    change = -1;
                    dentedCount--;
                    cerr << "Dented Count: " << dentedCount << endl;
                }
                //cerr << "Count: " << count << endl;
            }else {
                //this should not really happen - unless if spinning anti-clockwise too fast
                cerr << "Delta: " << delta << endl;
            }
            prevSeq = seq;
            FMCManager::getInstance()->rotaryEncTurnEvent(count, change);
        }
        usleep(100);		
	}
	cerr << "Finished RotaryEncScanner::mainLoop" << endl;
}
