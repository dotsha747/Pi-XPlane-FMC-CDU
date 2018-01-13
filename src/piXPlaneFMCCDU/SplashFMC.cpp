/*
 * SplashFMC.cpp
 *
 *  Created on: Jan 8, 2018
 *      Author: shahada
 */

#include <sstream>

#include "Screen.h"
#include "Version.h"

#include "SplashFMC.h"

using namespace std;

SplashFMC::SplashFMC() {
	numCols = 24;
}

SplashFMC::~SplashFMC() {

}

void SplashFMC::init() {

	Screen::getInstance()->initialize(numCols);

	Screen::getInstance()->clearScreen();

	drawCenteredText(0, "Pi-XPlane-FMC-CDU");

	ostringstream buf;
	buf << "Ver " << MAJORVERSION << "." << MINORVERSION;
	drawCenteredText(3, buf.str());


	drawCenteredText(6, "Copyright (c) 2017-2018");
	drawCenteredText(7, "shahada abubakar");

	drawCenteredText (9, "No Warranty. Free S/W.");
	drawCenteredText (10, "{Refer to license for}");
	drawCenteredText (11, "redistribution terms");




}

void SplashFMC::deInit() {

}

void SplashFMC::receiveData(time_t time, std::string type, std::string dataref,
		std::string value) {

}

void SplashFMC::keyPressEvent(int row, int col) {

}

void SplashFMC::keyReleaseEvent(int row, int col) {

}

void SplashFMC::drawCenteredText(int line, std::string text) {

	int x = (numCols - text.length()) / 2;
	Screen::getInstance()->drawLine(x, line, text);

}
