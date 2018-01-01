//============================================================================
// Name        : Remote737FMCCDU2.cpp

// Author      : Shahada Abubakar
// Version     :
// Copyright   : Copyright (c) 2014, NEXTSense Sdn Bhd
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <wiringPi.h>


#include "ExtPlaneClient.h"
#include "Screen.h"
#include "KeypadScanner.h"
#include "FMCList.h"



//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

using namespace std;




int main(int argc, char * argv[]) {


	// initialize wiringPi
	wiringPiSetup ();


	Screen::getInstance()->init();
	ExtPlaneClient::getInstance()->init();
	FMCList::getInstance()->init();

	// launches background thread
	ExtPlaneClient::getInstance()->launchThread();
	KeypadScanner::getInstance()->launchThread();

	// main SDL loop
	Screen::getInstance()->mainLoop();


	return 0;
}



