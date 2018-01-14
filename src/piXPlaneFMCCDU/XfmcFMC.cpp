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

#include "Screen.h"

#include "FMCManager.h"
#include "LEDs.h"
#include "XfmcFMC.h"

using namespace std;

XfmcFMC::XfmcFMC() {

}

XfmcFMC::~XfmcFMC() {

}

void XfmcFMC::init() {

	Screen::getInstance()->initialize(36);

	// clear the screen, and wait for the lines to come in to draw the page
	Screen::getInstance()->clearScreen();

	LEDs::getInstance()->setAllLEDs(0);

	FMCManager::getInstance()->subscribeDataRef("xfmc/Upper");
	FMCManager::getInstance()->subscribeDataRef("xfmc/Panel_1");
	FMCManager::getInstance()->subscribeDataRef("xfmc/Keypath");
}



void XfmcFMC::deInit() {

	FMCManager::getInstance()->unsubscribeDataRef("xfmc/Upper");
	FMCManager::getInstance()->unsubscribeDataRef("xfmc/Panel_1");
	FMCManager::getInstance()->unsubscribeDataRef("xfmc/Keypath");

	FMCManager::getInstance()->setDataRef("xfmc/Keypath", "55");
}


void XfmcFMC::receiveDataRef(std::string type, std::string dataref, std::string value) {

	syslog (LOG_INFO, "type[%s] dataref[%s] value[%s]", type.c_str(), dataref.c_str(), value.c_str());

}


void XfmcFMC::keyPressEvent (int row, int col) {

}

void XfmcFMC::keyReleaseEvent (int row, int col) {

}
