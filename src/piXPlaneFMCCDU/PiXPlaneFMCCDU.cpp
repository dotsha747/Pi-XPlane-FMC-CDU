//============================================================================
// Name        : Remote737FMCCDU2.cpp

// Author      : Shahada Abubakar
// Version     :
// Copyright   : Copyright (c) 2014, NEXTSense Sdn Bhd
// Description : Hello World in C++, Ansi-style
//============================================================================


#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <wiringPi.h>

#include "FMCManager.h"

#include "Screen.h"
#include "KeypadScanner.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

using namespace std;


int main(int argc, char * argv[]) {

	syslog (LOG_INFO, "=== PiXPlaneFMCCDU is starting. ===");

	// initialize
	FMCManager::getInstance();
	KeypadScanner::getInstance()->launchThread();

	// main SDL loop
	Screen::getInstance()->mainLoop();


	return 0;
}



