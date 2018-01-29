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
#include <regex>
#include <sstream>
#include <algorithm>

#include "Base64.h"

#include "ZiboFMC.h"
#include "Screen.h"
#include "FMCManager.h"
#include "LEDs.h"

using namespace std;

ZiboFMC::ZiboFMC(bool isCaptain) {

	side = isCaptain ? "1" : "2";

	// populate keyInfo
	keyInfo[1][1] = "laminar/B738/button/fmc" + side + "_1L";	// sw-1
	keyInfo[2][1] = "laminar/B738/button/fmc" + side + "_2L";	// sw-2
	keyInfo[3][1] = "laminar/B738/button/fmc" + side + "_3L"; // sw-3
	keyInfo[4][1] = "laminar/B738/button/fmc" + side + "_4L"; 	// sw-4
	keyInfo[5][1] = "laminar/B738/button/fmc" + side + "_5L"; 	// sw-5
	keyInfo[6][1] = "laminar/B738/button/fmc" + side + "_6L"; 	// sw-6
	keyInfo[7][1] = "laminar/B738/button/fmc" + side + "_1R"; 	// sw-7
	keyInfo[8][1] = "laminar/B738/button/fmc" + side + "_2R"; 	// sw-8
	keyInfo[1][2] = "laminar/B738/button/fmc" + side + "_3R"; 	// sw-9
	keyInfo[2][2] = "laminar/B738/button/fmc" + side + "_4R";	// sw-10
	keyInfo[3][2] = "laminar/B738/button/fmc" + side + "_5R"; 	// sw-11
	keyInfo[4][2] = "laminar/B738/button/fmc" + side + "_6R"; 	// sw-12
	keyInfo[5][2] = "laminar/B738/button/fmc" + side + "_init_ref"; 	// sw-13
	keyInfo[6][2] = "laminar/B738/button/fmc" + side + "_rte";	// sw-14
	keyInfo[7][2] = "laminar/B738/button/fmc" + side + "_clb"; 	// sw-15
	keyInfo[8][2] = "laminar/B738/button/fmc" + side + "_crz";	// sw-16
	keyInfo[1][3] = "laminar/B738/button/fmc" + side + "_des";	// sw-17
	keyInfo[2][3] = "laminar/B738/button/fmc" + side + "_menu";	// sw-18
	keyInfo[3][3] = "laminar/B738/button/fmc" + side + "_legs"; // sw-19
	keyInfo[4][3] = "laminar/B738/button/fmc" + side + "_dep_app";	// sw-20
	keyInfo[5][3] = "laminar/B738/button/fmc" + side + "_hold";	// sw-21
	keyInfo[6][3] = "laminar/B738/button/fmc" + side + "_prog";	// sw-22
	keyInfo[7][3] = "laminar/B738/button/fmc" + side + "_exec";	// sw-23
	keyInfo[8][3] = "laminar/B738/button/fmc" + side + "_n1_lim"; // sw-24
	keyInfo[1][4] = "laminar/B738/button/fmc" + side + "_fix";	// sw-25
	keyInfo[2][4] = "laminar/B738/button/fmc" + side + "_prev_page";	// sw-26
	keyInfo[3][4] = "laminar/B738/button/fmc" + side + "_next_page"; // sw-27
	keyInfo[4][4] = "laminar/B738/button/fmc" + side + "_A";	// sw-28
	keyInfo[5][4] = "laminar/B738/button/fmc" + side + "_B";	// sw-29
	keyInfo[6][4] = "laminar/B738/button/fmc" + side + "_C";	// sw-30
	keyInfo[7][4] = "laminar/B738/button/fmc" + side + "_D";	// sw-31
	keyInfo[8][4] = "laminar/B738/button/fmc" + side + "_E";	// sw32
	keyInfo[1][5] = "laminar/B738/button/fmc" + side + "_F";	// sw-33
	keyInfo[2][5] = "laminar/B738/button/fmc" + side + "_G";	// sw-34
	keyInfo[3][5] = "laminar/B738/button/fmc" + side + "_H";	// sw-35
	keyInfo[4][5] = "laminar/B738/button/fmc" + side + "_I";	// sw-36
	keyInfo[5][5] = "laminar/B738/button/fmc" + side + "_J";	// sw-37
	keyInfo[6][5] = "laminar/B738/button/fmc" + side + "_K";	// sw-38
	keyInfo[7][5] = "laminar/B738/button/fmc" + side + "_L";	// sw-39
	keyInfo[8][5] = "laminar/B738/button/fmc" + side + "_M";	// sw-40
	keyInfo[1][6] = "laminar/B738/button/fmc" + side + "_N";	// sw-41
	keyInfo[2][6] = "laminar/B738/button/fmc" + side + "_O";	// sw-42
	keyInfo[3][6] = "laminar/B738/button/fmc" + side + "_P";	// sw-43
	keyInfo[4][6] = "laminar/B738/button/fmc" + side + "_Q";	// sw-44
	keyInfo[5][6] = "laminar/B738/button/fmc" + side + "_R";	// sw-45
	keyInfo[6][6] = "laminar/B738/button/fmc" + side + "_S";	// sw-46
	keyInfo[7][6] = "laminar/B738/button/fmc" + side + "_T";	// sw-47
	keyInfo[8][6] = "laminar/B738/button/fmc" + side + "_U";	// sw-48
	keyInfo[1][7] = "laminar/B738/button/fmc" + side + "_V";	// sw-49
	keyInfo[2][7] = "laminar/B738/button/fmc" + side + "_W";	// sw-50
	keyInfo[3][7] = "laminar/B738/button/fmc" + side + "_X";	// sw-51
	keyInfo[4][7] = "laminar/B738/button/fmc" + side + "_Y";	// sw-52
	keyInfo[5][7] = "laminar/B738/button/fmc" + side + "_Z";	// sw-53
	keyInfo[6][7] = "laminar/B738/button/fmc" + side + "_SP";	// sw-54
	keyInfo[7][7] = "laminar/B738/button/fmc" + side + "_del";		// sw-55
	keyInfo[8][7] = "laminar/B738/button/fmc" + side + "_slash";	// sw-56
	keyInfo[1][8] = "laminar/B738/button/fmc" + side + "_clr";		// sw-57
	keyInfo[2][8] = "laminar/B738/button/fmc" + side + "_period";	// sw-58
	keyInfo[3][8] = "laminar/B738/button/fmc" + side + "_0";	// sw-59
	keyInfo[4][8] = "laminar/B738/button/fmc" + side + "_minus";	// sw-60
	keyInfo[5][8] = "laminar/B738/button/fmc" + side + "_7"; 	// sw-61
	keyInfo[6][8] = "laminar/B738/button/fmc" + side + "_8";	// sw-62
	keyInfo[7][8] = "laminar/B738/button/fmc" + side + "_9";	// sw-63
	keyInfo[8][8] = "laminar/B738/button/fmc" + side + "_4";	// sw-64
	keyInfo[1][9] = "laminar/B738/button/fmc" + side + "_5";	// sw-65
	keyInfo[2][9] = "laminar/B738/button/fmc" + side + "_6";	// sw-66
	keyInfo[3][9] = "laminar/B738/button/fmc" + side + "_1";	// sw-67
	keyInfo[4][9] = "laminar/B738/button/fmc" + side + "_2";	// sw-68
	keyInfo[5][9] = "laminar/B738/button/fmc" + side + "_3";	// sw-69

	xplaneUDPClient = NULL;

}

ZiboFMC::~ZiboFMC() {
	// TODO Auto-generated destructor stub
}

std::string ZiboFMC::getName() {

	return side == "1" ? "ZIB0" : "ZIB1";
}

void ZiboFMC::init() {

	Screen::getInstance()->initialize(24);
	Screen::getInstance()->clearScreen();
	LEDs::getInstance()->setAllLEDs(0);
	subscribeDataRefs();
}


/** @brief hack to workaround a extplane/zibo bug
 *
 */

void ZiboFMC::initSetHost (std::string host, int port) {

	xplaneUDPClient = new XPlaneUDPClient (
			host, 49000,
		std::bind (&ZiboFMC::receiveUDPDataRefFloat, this, std::placeholders::_1, std::placeholders::_2),
		std::bind (&ZiboFMC::receiveUDPDataRefString, this, std::placeholders::_1, std::placeholders::_2)

	);

	xplaneUDPClient->subscribeDataRef("laminar/B738/fmc" + side + "/Line_entry[0][23]", 3);

}


void ZiboFMC::subscribe (std::string dataref) {

	FMCManager::getInstance()->subscribeDataRef (dataref);
	//xplaneUDPClient->subscribeDataRef(dataref+"[0][23]", 3);
}

void ZiboFMC::unsubscribe(std::string dataref) {

	FMCManager::getInstance()->unsubscribeDataRef(dataref);
}

void ZiboFMC::subscribeDataRefs() {

	// These are the datarefs that we need when we're active.

	// L = Large Lines, including Page Label (maps to even line numbers)
	// X = Small Lines (maps to odd line numbers)
	// S = small font on Large Lines

	subscribe("laminar/B738/fmc" + side + "/Line00_L");
	subscribe("laminar/B738/fmc" + side + "/Line00_S");

	subscribe("laminar/B738/fmc" + side + "/Line01_X");
	subscribe("laminar/B738/fmc" + side + "/Line02_X");
	subscribe("laminar/B738/fmc" + side + "/Line03_X");
	subscribe("laminar/B738/fmc" + side + "/Line04_X");
	subscribe("laminar/B738/fmc" + side + "/Line05_X");
	subscribe("laminar/B738/fmc" + side + "/Line06_X");

	subscribe("laminar/B738/fmc" + side + "/Line01_L");
	subscribe("laminar/B738/fmc" + side + "/Line02_L");
	subscribe("laminar/B738/fmc" + side + "/Line03_L");
	subscribe("laminar/B738/fmc" + side + "/Line04_L");
	subscribe("laminar/B738/fmc" + side + "/Line05_L");
	subscribe("laminar/B738/fmc" + side + "/Line06_L");
	subscribe("laminar/B738/fmc" + side + "/Line01_I");
	subscribe("laminar/B738/fmc" + side + "/Line02_I");
	subscribe("laminar/B738/fmc" + side + "/Line03_I");
	subscribe("laminar/B738/fmc" + side + "/Line04_I");
	subscribe("laminar/B738/fmc" + side + "/Line05_I");
	subscribe("laminar/B738/fmc" + side + "/Line06_I");
	subscribe("laminar/B738/fmc" + side + "/Line01_S");
	subscribe("laminar/B738/fmc" + side + "/Line02_S");
	subscribe("laminar/B738/fmc" + side + "/Line03_S");
	subscribe("laminar/B738/fmc" + side + "/Line04_S");
	subscribe("laminar/B738/fmc" + side + "/Line05_S");
	subscribe("laminar/B738/fmc" + side + "/Line06_S");
	subscribe("laminar/B738/fmc" + side + "/Line01_G");
	subscribe("laminar/B738/fmc" + side + "/Line02_G");
	subscribe("laminar/B738/fmc" + side + "/Line03_G");
	subscribe("laminar/B738/fmc" + side + "/Line04_G");
	subscribe("laminar/B738/fmc" + side + "/Line05_G");
	subscribe("laminar/B738/fmc" + side + "/Line06_G");
	subscribe("laminar/B738/fmc" + side + "/Line01_M");
	subscribe("laminar/B738/fmc" + side + "/Line02_M");
	subscribe("laminar/B738/fmc" + side + "/Line03_M");
	subscribe("laminar/B738/fmc" + side + "/Line04_M");
	subscribe("laminar/B738/fmc" + side + "/Line05_M");
	subscribe("laminar/B738/fmc" + side + "/Line06_M");

	if (side=="0") {
		subscribe("laminar/B738/indicators/fmc_exec_lights");
	} else {
		subscribe("laminar/B738/indicators/fmc_exec_lights_fo");
	}

}


void ZiboFMC::deInit () {


	unsubscribe("laminar/B738/fmc" + side + "/Line00_L");
	unsubscribe("laminar/B738/fmc" + side + "/Line00_S");

	unsubscribe("laminar/B738/fmc" + side + "/Line01_X");
	unsubscribe("laminar/B738/fmc" + side + "/Line02_X");
	unsubscribe("laminar/B738/fmc" + side + "/Line03_X");
	unsubscribe("laminar/B738/fmc" + side + "/Line04_X");
	unsubscribe("laminar/B738/fmc" + side + "/Line05_X");
	unsubscribe("laminar/B738/fmc" + side + "/Line06_X");

	unsubscribe("laminar/B738/fmc" + side + "/Line01_L");
	unsubscribe("laminar/B738/fmc" + side + "/Line02_L");
	unsubscribe("laminar/B738/fmc" + side + "/Line03_L");
	unsubscribe("laminar/B738/fmc" + side + "/Line04_L");
	unsubscribe("laminar/B738/fmc" + side + "/Line05_L");
	unsubscribe("laminar/B738/fmc" + side + "/Line06_L");
	unsubscribe("laminar/B738/fmc" + side + "/Line01_I");
	unsubscribe("laminar/B738/fmc" + side + "/Line02_I");
	unsubscribe("laminar/B738/fmc" + side + "/Line03_I");
	unsubscribe("laminar/B738/fmc" + side + "/Line04_I");
	unsubscribe("laminar/B738/fmc" + side + "/Line05_I");
	unsubscribe("laminar/B738/fmc" + side + "/Line06_I");
	unsubscribe("laminar/B738/fmc" + side + "/Line01_S");
	unsubscribe("laminar/B738/fmc" + side + "/Line02_S");
	unsubscribe("laminar/B738/fmc" + side + "/Line03_S");
	unsubscribe("laminar/B738/fmc" + side + "/Line04_S");
	unsubscribe("laminar/B738/fmc" + side + "/Line05_S");
	unsubscribe("laminar/B738/fmc" + side + "/Line06_S");
	unsubscribe("laminar/B738/fmc" + side + "/Line01_G");
	unsubscribe("laminar/B738/fmc" + side + "/Line02_G");
	unsubscribe("laminar/B738/fmc" + side + "/Line03_G");
	unsubscribe("laminar/B738/fmc" + side + "/Line04_G");
	unsubscribe("laminar/B738/fmc" + side + "/Line05_G");
	unsubscribe("laminar/B738/fmc" + side + "/Line06_G");
	unsubscribe("laminar/B738/fmc" + side + "/Line01_M");
	unsubscribe("laminar/B738/fmc" + side + "/Line02_M");
	unsubscribe("laminar/B738/fmc" + side + "/Line03_M");
	unsubscribe("laminar/B738/fmc" + side + "/Line04_M");
	unsubscribe("laminar/B738/fmc" + side + "/Line05_M");
	unsubscribe("laminar/B738/fmc" + side + "/Line06_M");

	if (side=="0") {
		unsubscribe("laminar/B738/indicators/fmc_exec_lights");
	} else {
		unsubscribe("laminar/B738/indicators/fmc_exec_lights_fo");
	}
	if (xplaneUDPClient != NULL) {
		delete xplaneUDPClient;
		xplaneUDPClient = NULL;
	}

}

void ZiboFMC::keyPressEvent(int row, int col) {
	auto rowV = keyInfo.find(row);
	if (rowV == keyInfo.end()) {
		cerr << "Invalid row " << row << endl;
		return;
	}

	auto colV = rowV->second.find(col);
	if (colV == rowV->second.end()) {
		cerr << "Invalid col " << col << endl;
		return;
	}

	cerr << "zibo keypress on r=" << rowV->first << " c=" << colV->first
			<< " \"" << colV->second << "\"" << endl;

	FMCManager::getInstance()->sendCommand(colV->second);
}

void ZiboFMC::keyReleaseEvent(int row, int col) {

	// do nothing?
}


void ZiboFMC::receiveDataRef(std::string type, std::string dataref,
		std::string value) {

	ostringstream buf;
	buf << "ZiboFMC got [" << dataref << "|" << value << "]" << endl;
	cerr << buf.str() << endl;
	syslog (LOG_INFO, "%s", buf.str().c_str());


	regex r("^laminar/B738/fmc" + side + "/Line(\\d+)_(.)$");
	smatch m;
	if (regex_match(dataref, m, r)) {

		int line = stoi(m[1]);
		string size = m[2];

		// zibo uses '*' for the square entry box. we don't have that
		// so we use '_' instead.
		replace (value.begin(), value.end(), '*', '_');

		/* Zibo has a number of datarefs that apply to every line.
		 * 	X=Small Lines
		 * 	L=Large
		 * 	-->G=Green Text in Large Lines
		 * 	-->M=Magenta Text in Large Lines
		 * 	-->I=Inverse Text in Large Lines
		 * 	-->S=Small Text in Large Lines
		 */

		// normal large lines
		if (size == "L") {

			// new page
			if (line == 0) {
				Screen::getInstance()->clearScreen();
			}

			line = (line) * 2;
			Screen::getInstance()->drawLine(0, line, value);

		}

		// normal small lines
		else if (size == "X") {
			line = 1 + ((line - 1) * 2);
			Screen::getInstance()->drawLine(0, line, value);
		}

		// overlay small font in large line overlay
		else if (size == "S") {
			line = (line) * 2;
			Screen::getInstance()->drawLine(0, line, value, false);
		}

		// overlay magenta font in large line
		else if (size == "M") {
			line = (line) * 2;
			Screen::getInstance()->drawLine(0, line, value, false, 'M');
		}

		// overlay green font in large line
		else if (size == "G") {
			line = (line) * 2;
			Screen::getInstance()->drawLine(0, line, value, false, 'G');
		}

	}

	else if (dataref == "laminar/B738/fmc" + side + "/Line_entry") {
		Screen::getInstance()->drawLine (0, 13, value);
		cerr << "Line_entry [" << value << "]" << endl;

	}

	else if (dataref == "laminar/B738/indicators/fmc_exec_lights" || dataref == "laminar/B738/indicators/fmc_exec_lights_fo") {
		LEDs::getInstance()->setLED(LEDs::LED_EXEC, value != "0");
	}

}


void ZiboFMC::receiveUDPDataRefFloat (std::string dataref, float value) {

}

void ZiboFMC::receiveUDPDataRefString (std::string dataref, std::string value) {

	cerr << "ZIBO UDP STR [" << dataref << "] [" << value << "]" << endl;

	// pad the value to 24 columns
	if (value.size() < 24) {
		value.insert (value.end(), 24-value.size(), ' ');
	}

	// strip the indices out of the dataref
	std::size_t indpos = dataref.find_first_of('[');
	if (indpos != string::npos) {
		dataref.erase (indpos);
	}

	receiveDataRef ("ub", dataref, value);

}





/** @brief Format lines that have large and small text.
 *
 * 	Zibo handles lines with mixed sizes in a weird way, they are stored in
 * 	separate datarefs, those ending with L (for the normal large fonts) and
 * 	those ending with S (for the smallfonts). Normally where only large fonts
 * 	appear, only L datarefs are transmitted. If a line contains mixed fonts,
 * 	we will receive and L dataref followed by an S dataref. Assumption: we
 * 	will never get an S without first receiving an L.
 *
 *
 */

std::string ZiboFMC::mixLargeSmallLines(std::string large, std::string small) {

	ostringstream buf;

	for (unsigned int i = 0; i < small.length(); i++) {

		if (small[i] == ' ') {
			// nothing from small, so take from big
			buf << large[i];
		} else {
			// something in small, so take from here. No one said we can't
			// wrap each character 8-)
			buf << "{" << small[i] << "}";
		}
	}

	// cerr << "Mixing [" << large << "] with [" << small << "] and got [" << buf.str() << "]" << endl;

	return buf.str();

}
