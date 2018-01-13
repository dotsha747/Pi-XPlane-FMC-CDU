/*
 * ZiboFMC.cpp
 *
 *  Created on: Jul 26, 2017
 *      Author: shahada
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

ZiboFMC::ZiboFMC() {

	// populate keyInfo
	keyInfo[1][1] = "laminar/B738/button/fmc1_1L";	// sw-1
	keyInfo[2][1] = "laminar/B738/button/fmc1_2L";	// sw-2
	keyInfo[3][1] = "laminar/B738/button/fmc1_3L"; // sw-3
	keyInfo[4][1] = "laminar/B738/button/fmc1_4L"; 	// sw-4
	keyInfo[5][1] = "laminar/B738/button/fmc1_5L"; 	// sw-5
	keyInfo[6][1] = "laminar/B738/button/fmc1_6L"; 	// sw-6
	keyInfo[7][1] = "laminar/B738/button/fmc1_1R"; 	// sw-7
	keyInfo[8][1] = "laminar/B738/button/fmc1_2R"; 	// sw-8
	keyInfo[1][2] = "laminar/B738/button/fmc1_3R"; 	// sw-9
	keyInfo[2][2] = "laminar/B738/button/fmc1_4R";	// sw-10
	keyInfo[3][2] = "laminar/B738/button/fmc1_5R"; 	// sw-11
	keyInfo[4][2] = "laminar/B738/button/fmc1_6R"; 	// sw-12
	keyInfo[5][2] = "laminar/B738/button/fmc1_init_ref"; 	// sw-13
	keyInfo[6][2] = "laminar/B738/button/fmc1_rte";	// sw-14
	keyInfo[7][2] = "laminar/B738/button/fmc1_clb"; 	// sw-15
	keyInfo[8][2] = "laminar/B738/button/fmc1_crz";	// sw-16
	keyInfo[1][3] = "laminar/B738/button/fmc1_des";	// sw-17
	keyInfo[2][3] = "laminar/B738/button/fmc1_menu";	// sw-18
	keyInfo[3][3] = "laminar/B738/button/fmc1_legs"; // sw-19
	keyInfo[4][3] = "laminar/B738/button/fmc1_dep_app";	// sw-20
	keyInfo[5][3] = "laminar/B738/button/fmc1_hold";	// sw-21
	keyInfo[6][3] = "laminar/B738/button/fmc1_prog";	// sw-22
	keyInfo[7][3] = "laminar/B738/button/fmc1_exec";	// sw-23
	keyInfo[8][3] = "laminar/B738/button/fmc1_n1_lim"; // sw-24
	keyInfo[1][4] = "laminar/B738/button/fmc1_fix";	// sw-25
	keyInfo[2][4] = "laminar/B738/button/fmc1_prev_page";	// sw-26
	keyInfo[3][4] = "laminar/B738/button/fmc1_next_page"; // sw-27
	keyInfo[4][4] = "laminar/B738/button/fmc1_A";	// sw-28
	keyInfo[5][4] = "laminar/B738/button/fmc1_B";	// sw-29
	keyInfo[6][4] = "laminar/B738/button/fmc1_C";	// sw-30
	keyInfo[7][4] = "laminar/B738/button/fmc1_D";	// sw-31
	keyInfo[8][4] = "laminar/B738/button/fmc1_E";	// sw32
	keyInfo[1][5] = "laminar/B738/button/fmc1_F";	// sw-33
	keyInfo[2][5] = "laminar/B738/button/fmc1_G";	// sw-34
	keyInfo[3][5] = "laminar/B738/button/fmc1_H";	// sw-35
	keyInfo[4][5] = "laminar/B738/button/fmc1_I";	// sw-36
	keyInfo[5][5] = "laminar/B738/button/fmc1_J";	// sw-37
	keyInfo[6][5] = "laminar/B738/button/fmc1_K";	// sw-38
	keyInfo[7][5] = "laminar/B738/button/fmc1_L";	// sw-39
	keyInfo[8][5] = "laminar/B738/button/fmc1_M";	// sw-40
	keyInfo[1][6] = "laminar/B738/button/fmc1_N";	// sw-41
	keyInfo[2][6] = "laminar/B738/button/fmc1_O";	// sw-42
	keyInfo[3][6] = "laminar/B738/button/fmc1_P";	// sw-43
	keyInfo[4][6] = "laminar/B738/button/fmc1_Q";	// sw-44
	keyInfo[5][6] = "laminar/B738/button/fmc1_R";	// sw-45
	keyInfo[6][6] = "laminar/B738/button/fmc1_S";	// sw-46
	keyInfo[7][6] = "laminar/B738/button/fmc1_T";	// sw-47
	keyInfo[8][6] = "laminar/B738/button/fmc1_U";	// sw-48
	keyInfo[1][7] = "laminar/B738/button/fmc1_V";	// sw-49
	keyInfo[2][7] = "laminar/B738/button/fmc1_W";	// sw-50
	keyInfo[3][7] = "laminar/B738/button/fmc1_X";	// sw-51
	keyInfo[4][7] = "laminar/B738/button/fmc1_Y";	// sw-52
	keyInfo[5][7] = "laminar/B738/button/fmc1_Z";	// sw-53
	keyInfo[6][7] = "laminar/B738/button/fmc1_SP";	// sw-54
	keyInfo[7][7] = "laminar/B738/button/fmc1_del";		// sw-55
	keyInfo[8][7] = "laminar/B738/button/fmc1_slash";	// sw-56
	keyInfo[1][8] = "laminar/B738/button/fmc1_clr";		// sw-57
	keyInfo[2][8] = "laminar/B738/button/fmc1_period";	// sw-58
	keyInfo[3][8] = "laminar/B738/button/fmc1_0";	// sw-59
	keyInfo[4][8] = "laminar/B738/button/fmc1_minus";	// sw-60
	keyInfo[5][8] = "laminar/B738/button/fmc1_7"; 	// sw-61
	keyInfo[6][8] = "laminar/B738/button/fmc1_8";	// sw-62
	keyInfo[7][8] = "laminar/B738/button/fmc1_9";	// sw-63
	keyInfo[8][8] = "laminar/B738/button/fmc1_4";	// sw-64
	keyInfo[1][9] = "laminar/B738/button/fmc1_5";	// sw-65
	keyInfo[2][9] = "laminar/B738/button/fmc1_6";	// sw-66
	keyInfo[3][9] = "laminar/B738/button/fmc1_1";	// sw-67
	keyInfo[4][9] = "laminar/B738/button/fmc1_2";	// sw-68
	keyInfo[5][9] = "laminar/B738/button/fmc1_3";	// sw-69

	xplaneUDPClient = NULL;

}

ZiboFMC::~ZiboFMC() {
	// TODO Auto-generated destructor stub
}

std::string ZiboFMC::getName() {

	return "ZIBO";
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

	xplaneUDPClient->subscribeDataRef("laminar/B738/fmc1/Line_entry[0][23]", 3);

}


void ZiboFMC::subscribeDataRefs() {

	// These are the datarefs that we need when we're active.

	// L = Large Lines, including Page Label (maps to even line numbers)
	// X = Small Lines (maps to odd line numbers)
	// S = small font on Large Lines

	FMCManager::getInstance()->subscribeDataRef("laminar/B738/fmc1/Line00_L", 1);
	FMCManager::getInstance()->subscribeDataRef("laminar/B738/fmc1/Line00_S", 1);

	FMCManager::getInstance()->subscribeDataRef("laminar/B738/fmc1/Line01_X", 1);
	FMCManager::getInstance()->subscribeDataRef("laminar/B738/fmc1/Line02_X", 1);
	FMCManager::getInstance()->subscribeDataRef("laminar/B738/fmc1/Line03_X", 1);
	FMCManager::getInstance()->subscribeDataRef("laminar/B738/fmc1/Line04_X", 1);
	FMCManager::getInstance()->subscribeDataRef("laminar/B738/fmc1/Line05_X", 1);
	FMCManager::getInstance()->subscribeDataRef("laminar/B738/fmc1/Line06_X", 1);

	FMCManager::getInstance()->subscribeDataRef("laminar/B738/fmc1/Line01_L", 1);
	FMCManager::getInstance()->subscribeDataRef("laminar/B738/fmc1/Line02_L", 1);
	FMCManager::getInstance()->subscribeDataRef("laminar/B738/fmc1/Line03_L", 1);
	FMCManager::getInstance()->subscribeDataRef("laminar/B738/fmc1/Line04_L", 1);
	FMCManager::getInstance()->subscribeDataRef("laminar/B738/fmc1/Line05_L", 1);
	FMCManager::getInstance()->subscribeDataRef("laminar/B738/fmc1/Line06_L", 1);
	FMCManager::getInstance()->subscribeDataRef("laminar/B738/fmc1/Line01_I", 1);
	FMCManager::getInstance()->subscribeDataRef("laminar/B738/fmc1/Line02_I", 1);
	FMCManager::getInstance()->subscribeDataRef("laminar/B738/fmc1/Line03_I", 1);
	FMCManager::getInstance()->subscribeDataRef("laminar/B738/fmc1/Line04_I", 1);
	FMCManager::getInstance()->subscribeDataRef("laminar/B738/fmc1/Line05_I", 1);
	FMCManager::getInstance()->subscribeDataRef("laminar/B738/fmc1/Line06_I", 1);
	FMCManager::getInstance()->subscribeDataRef("laminar/B738/fmc1/Line01_S", 1);
	FMCManager::getInstance()->subscribeDataRef("laminar/B738/fmc1/Line02_S", 1);
	FMCManager::getInstance()->subscribeDataRef("laminar/B738/fmc1/Line03_S", 1);
	FMCManager::getInstance()->subscribeDataRef("laminar/B738/fmc1/Line04_S", 1);
	FMCManager::getInstance()->subscribeDataRef("laminar/B738/fmc1/Line05_S", 1);
	FMCManager::getInstance()->subscribeDataRef("laminar/B738/fmc1/Line06_S", 1);
	FMCManager::getInstance()->subscribeDataRef("laminar/B738/fmc1/Line01_G", 1);
	FMCManager::getInstance()->subscribeDataRef("laminar/B738/fmc1/Line02_G", 1);
	FMCManager::getInstance()->subscribeDataRef("laminar/B738/fmc1/Line03_G", 1);
	FMCManager::getInstance()->subscribeDataRef("laminar/B738/fmc1/Line04_G", 1);
	FMCManager::getInstance()->subscribeDataRef("laminar/B738/fmc1/Line05_G", 1);
	FMCManager::getInstance()->subscribeDataRef("laminar/B738/fmc1/Line06_G", 1);
	FMCManager::getInstance()->subscribeDataRef("laminar/B738/fmc1/Line01_M", 1);
	FMCManager::getInstance()->subscribeDataRef("laminar/B738/fmc1/Line02_M", 1);
	FMCManager::getInstance()->subscribeDataRef("laminar/B738/fmc1/Line03_M", 1);
	FMCManager::getInstance()->subscribeDataRef("laminar/B738/fmc1/Line04_M", 1);
	FMCManager::getInstance()->subscribeDataRef("laminar/B738/fmc1/Line05_M", 1);
	FMCManager::getInstance()->subscribeDataRef("laminar/B738/fmc1/Line06_M", 1);

	FMCManager::getInstance()->subscribeDataRef("laminar/B738/indicators/fmc_exec_lights", 1);

}


void ZiboFMC::deInit () {
	FMCManager::getInstance()->unsubscribeDataRef("laminar/B738/fmc1/Line00_L");
	FMCManager::getInstance()->unsubscribeDataRef("laminar/B738/fmc1/Line00_S");

	FMCManager::getInstance()->unsubscribeDataRef("laminar/B738/fmc1/Line01_X");
	FMCManager::getInstance()->unsubscribeDataRef("laminar/B738/fmc1/Line02_X");
	FMCManager::getInstance()->unsubscribeDataRef("laminar/B738/fmc1/Line03_X");
	FMCManager::getInstance()->unsubscribeDataRef("laminar/B738/fmc1/Line04_X");
	FMCManager::getInstance()->unsubscribeDataRef("laminar/B738/fmc1/Line05_X");
	FMCManager::getInstance()->unsubscribeDataRef("laminar/B738/fmc1/Line06_X");

	FMCManager::getInstance()->unsubscribeDataRef("laminar/B738/fmc1/Line01_L");
	FMCManager::getInstance()->unsubscribeDataRef("laminar/B738/fmc1/Line02_L");
	FMCManager::getInstance()->unsubscribeDataRef("laminar/B738/fmc1/Line03_L");
	FMCManager::getInstance()->unsubscribeDataRef("laminar/B738/fmc1/Line04_L");
	FMCManager::getInstance()->unsubscribeDataRef("laminar/B738/fmc1/Line05_L");
	FMCManager::getInstance()->unsubscribeDataRef("laminar/B738/fmc1/Line06_L");
	FMCManager::getInstance()->unsubscribeDataRef("laminar/B738/fmc1/Line01_I");
	FMCManager::getInstance()->unsubscribeDataRef("laminar/B738/fmc1/Line02_I");
	FMCManager::getInstance()->unsubscribeDataRef("laminar/B738/fmc1/Line03_I");
	FMCManager::getInstance()->unsubscribeDataRef("laminar/B738/fmc1/Line04_I");
	FMCManager::getInstance()->unsubscribeDataRef("laminar/B738/fmc1/Line05_I");
	FMCManager::getInstance()->unsubscribeDataRef("laminar/B738/fmc1/Line06_I");
	FMCManager::getInstance()->unsubscribeDataRef("laminar/B738/fmc1/Line01_S");
	FMCManager::getInstance()->unsubscribeDataRef("laminar/B738/fmc1/Line02_S");
	FMCManager::getInstance()->unsubscribeDataRef("laminar/B738/fmc1/Line03_S");
	FMCManager::getInstance()->unsubscribeDataRef("laminar/B738/fmc1/Line04_S");
	FMCManager::getInstance()->unsubscribeDataRef("laminar/B738/fmc1/Line05_S");
	FMCManager::getInstance()->unsubscribeDataRef("laminar/B738/fmc1/Line06_S");
	FMCManager::getInstance()->unsubscribeDataRef("laminar/B738/fmc1/Line01_G");
	FMCManager::getInstance()->unsubscribeDataRef("laminar/B738/fmc1/Line02_G");
	FMCManager::getInstance()->unsubscribeDataRef("laminar/B738/fmc1/Line03_G");
	FMCManager::getInstance()->unsubscribeDataRef("laminar/B738/fmc1/Line04_G");
	FMCManager::getInstance()->unsubscribeDataRef("laminar/B738/fmc1/Line05_G");
	FMCManager::getInstance()->unsubscribeDataRef("laminar/B738/fmc1/Line06_G");
	FMCManager::getInstance()->unsubscribeDataRef("laminar/B738/fmc1/Line01_M");
	FMCManager::getInstance()->unsubscribeDataRef("laminar/B738/fmc1/Line02_M");
	FMCManager::getInstance()->unsubscribeDataRef("laminar/B738/fmc1/Line03_M");
	FMCManager::getInstance()->unsubscribeDataRef("laminar/B738/fmc1/Line04_M");
	FMCManager::getInstance()->unsubscribeDataRef("laminar/B738/fmc1/Line05_M");
	FMCManager::getInstance()->unsubscribeDataRef("laminar/B738/fmc1/Line06_M");

	FMCManager::getInstance()->unsubscribeDataRef("laminar/B738/indicators/fmc_exec_lights");

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
	syslog (LOG_INFO, "%s", buf.str().c_str());


	regex r("^laminar/B738/fmc1/Line(\\d+)_(.)$");
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

	else if (dataref == "laminar/B738/fmc1/Line_entry") {
		Screen::getInstance()->drawLine (0, 13, value);
		cerr << "Line_entry [" << value << "]" << endl;

	}

	else if (dataref == "laminar/B738/indicators/fmc_exec_lights") {
		LEDs::getInstance()->setLED(LEDs::LED_EXEC, value != "0");
	}

}


void ZiboFMC::receiveUDPDataRefFloat (std::string dataref, float value) {

}

void ZiboFMC::receiveUDPDataRefString (std::string dataref, std::string value) {

	syslog (LOG_INFO, "ZIBO UDP STR [%s] [%s]", dataref.c_str(), value.c_str());

	// pad the value to 24 columns
	if (value.size() < 24) {
		value.insert (value.end(), 24-value.size(), ' ');
	}

	receiveDataRef ("ub", "laminar/B738/fmc1/Line_entry", value);

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
