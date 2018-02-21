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
#include <sstream>
#include <regex>

#include "LEDs.h"
#include "X737FMC.h"
#include "Screen.h"


#include "FMCManager.h"

using namespace std;

X737FMC::X737FMC() {

	// populate keyInfo
	keyInfo[1][1] = "FJCC/UFMC/LK1";	// sw-1
	keyInfo[2][1] = "FJCC/UFMC/LK2";	// sw-2
	keyInfo[3][1] = "FJCC/UFMC/LK3"; // sw-3
	keyInfo[4][1] = "FJCC/UFMC/LK4"; 	// sw-4
	keyInfo[5][1] = "FJCC/UFMC/LK5"; 	// sw-5
	keyInfo[6][1] = "FJCC/UFMC/LK6"; 	// sw-6
	keyInfo[7][1] = "FJCC/UFMC/RK1"; 	// sw-7
	keyInfo[8][1] = "FJCC/UFMC/RK2"; 	// sw-8
	keyInfo[1][2] = "FJCC/UFMC/RK3"; 	// sw-9
	keyInfo[2][2] = "FJCC/UFMC/RK4";	// sw-10
	keyInfo[3][2] = "FJCC/UFMC/RK5"; 	// sw-11
	keyInfo[4][2] = "FJCC/UFMC/RK6"; 	// sw-12
	keyInfo[5][2] = "FJCC/UFMC/INITREF"; 	// sw-13
	keyInfo[6][2] = "FJCC/UFMC/RTE";	// sw-14
	keyInfo[7][2] = "FJCC/UFMC/CLB_x737"; 	// sw-15
	keyInfo[8][2] = "FJCC/UFMC/CRZ_x737";	// sw-16
	keyInfo[1][3] = "FJCC/UFMC/DES";	// sw-17
	keyInfo[2][3] = "FJCC/UFMC/MENU";	// sw-18
	keyInfo[3][3] = "FJCC/UFMC/LEGS"; // sw-19
	keyInfo[4][3] = "FJCC/UFMC/DEPARR";	// sw-20
	keyInfo[5][3] = "FJCC/UFMC/HOLD";	// sw-21
	keyInfo[6][3] = "FJCC/UFMC/PROG";	// sw-22
	keyInfo[7][3] = "FJCC/UFMC/EXEC";	// sw-23
	keyInfo[8][3] = "FJCC/UFMC/N1"; // sw-24 NOTE: THIS IS MISSING FROM x737FMC. Have informed author.
	keyInfo[1][4] = "FJCC/UFMC/FIX";	// sw-25
	keyInfo[2][4] = "FJCC/UFMC/PREVPAGE";	// sw-26
	keyInfo[3][4] = "FJCC/UFMC/NEXTPAGE"; // sw-27
	keyInfo[4][4] = "FJCC/UFMC/A";	// sw-28
	keyInfo[5][4] = "FJCC/UFMC/B";	// sw-29
	keyInfo[6][4] = "FJCC/UFMC/C";	// sw-30
	keyInfo[7][4] = "FJCC/UFMC/D";	// sw-31
	keyInfo[8][4] = "FJCC/UFMC/E";	// sw32
	keyInfo[1][5] = "FJCC/UFMC/F";	// sw-33
	keyInfo[2][5] = "FJCC/UFMC/G";	// sw-34
	keyInfo[3][5] = "FJCC/UFMC/H";	// sw-35
	keyInfo[4][5] = "FJCC/UFMC/I";	// sw-36
	keyInfo[5][5] = "FJCC/UFMC/J";	// sw-37
	keyInfo[6][5] = "FJCC/UFMC/K";	// sw-38
	keyInfo[7][5] = "FJCC/UFMC/L";	// sw-39
	keyInfo[8][5] = "FJCC/UFMC/M";	// sw-40
	keyInfo[1][6] = "FJCC/UFMC/N";	// sw-41
	keyInfo[2][6] = "FJCC/UFMC/O";	// sw-42
	keyInfo[3][6] = "FJCC/UFMC/P";	// sw-43
	keyInfo[4][6] = "FJCC/UFMC/Q";	// sw-44
	keyInfo[5][6] = "FJCC/UFMC/R";	// sw-45
	keyInfo[6][6] = "FJCC/UFMC/S";	// sw-46
	keyInfo[7][6] = "FJCC/UFMC/T";	// sw-47
	keyInfo[8][6] = "FJCC/UFMC/U";	// sw-48
	keyInfo[1][7] = "FJCC/UFMC/V";	// sw-49
	keyInfo[2][7] = "FJCC/UFMC/W";	// sw-50
	keyInfo[3][7] = "FJCC/UFMC/X";	// sw-51
	keyInfo[4][7] = "FJCC/UFMC/Y";	// sw-52
	keyInfo[5][7] = "FJCC/UFMC/Z";	// sw-53
	keyInfo[6][7] = "FJCC/UFMC/espacio";	// sw-54
	keyInfo[7][7] = "FJCC/UFMC/DEL";		// sw-55
	keyInfo[8][7] = "FJCC/UFMC/barra";	// sw-56
	keyInfo[1][8] = "FJCC/UFMC/CLR";		// sw-57
	keyInfo[2][8] = "FJCC/UFMC/punto";	// sw-58
	keyInfo[3][8] = "FJCC/UFMC/0";	// sw-59
	keyInfo[4][8] = "FJCC/UFMC/menos";	// sw-60
	keyInfo[5][8] = "FJCC/UFMC/7"; 	// sw-61
	keyInfo[6][8] = "FJCC/UFMC/8";	// sw-62
	keyInfo[7][8] = "FJCC/UFMC/9";	// sw-63
	keyInfo[8][8] = "FJCC/UFMC/4";	// sw-64
	keyInfo[1][9] = "FJCC/UFMC/5";	// sw-65
	keyInfo[2][9] = "FJCC/UFMC/6";	// sw-66
	keyInfo[3][9] = "FJCC/UFMC/1";	// sw-67
	keyInfo[4][9] = "FJCC/UFMC/2";	// sw-68
	keyInfo[5][9] = "FJCC/UFMC/3";	// sw-69

	// calls constructor to init LEDs
	LEDs::getInstance();

	inLegsPage = false;

}

X737FMC::~X737FMC() {
	// TODO Auto-generated destructor stub
}

std::string X737FMC::getName() {
	return "X737";
}

void X737FMC::init() {

	// x737fmc has 25 columns!
	Screen::getInstance()->initialize(25);
	// clear the screen, and wait for the lines to come in to draw the page
	Screen::getInstance()->clearScreen();

	LEDs::getInstance()->setAllLEDs(0);

	subscribeDataRefs();
}

void X737FMC::subscribeDataRefs() {

	// we need to subscribe to all the keys to be able to update them
	for (auto rowV : keyInfo) {
		for (auto colV : rowV.second) {
			FMCManager::getInstance()->subscribeDataRef(colV.second, 1);
		}
	}

	// Subscribe to all the screen lines of the x737fmc.
	for (int line = 1; line <= 14; line++) {
		ostringstream buf;
		buf << "FJCC/UFMC/LINE_" << line;
		FMCManager::getInstance()->subscribeDataRef(buf.str(), 1);
	}

	// subscribe to datarefs for LEDs that x737fmc supports
	FMCManager::getInstance()->subscribeDataRef("FJCC/UFMC/Exec_Light_on", 1);
	FMCManager::getInstance()->subscribeDataRef("FJCC/UFMC/Offset_on", 1);

}

void X737FMC::deInit() {

	for (int line = 1; line <= 14; line++) {
		ostringstream buf;
		buf << "FJCC/UFMC/LINE_" << line;
		FMCManager::getInstance()->unsubscribeDataRef(buf.str());
	}

	// subscribe to datarefs for LEDs that x737fmc supports
	FMCManager::getInstance()->unsubscribeDataRef("FJCC/UFMC/Exec_Light_on");
	FMCManager::getInstance()->unsubscribeDataRef("FJCC/UFMC/Offset_on");

}

void X737FMC::keyPressEvent(int row, int col) {

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

	cerr << "x737fmc keypress on r=" << rowV->first << " c=" << colV->first
			<< " \"" << colV->second << "\"" << endl;

	FMCManager::getInstance()->setDataRef(colV->second, "1");

}

void X737FMC::keyReleaseEvent(int row, int col) {

	// x737 only deals with keyPressEvents, so do nothing.
}

void X737FMC::receiveDataRef(std::string type, std::string dataref,
		std::string value) {

	cerr << "X737fmc got [" << dataref << "|" << value << "]" << endl;

	// extract line number out
	regex r("^FJCC/UFMC/LINE_(\\d+)$");
	smatch m;
	if (regex_match(dataref, m, r)) {
		int line = stoi(m[1]);

		syslog(LOG_INFO, "QQ %d [%s]", line, value.c_str());

		if (line == 1) {
			inLegsPage = value.find("LEGS") != std::string::npos;
			syslog(LOG_INFO, "LEGS %s", inLegsPage ? "true" : "false");
		}

		if (inLegsPage) {

			if (line == 3 || line == 5 || line == 7 || line == 9 || line == 11) {

				// in legs page, the right side of each large line
				// is small by default. Just insert a $ to toggle
				// it.
				value.insert(13, "$");

				// x737 uses $...$ for small letters in big row.
				// replace it with {...}
				char sw = '{';
				for (int i = 0; i < value.size(); i++) {
					if (value[i] == '$') {
						value[i] = sw;
						sw = (sw == '{') ? '}' : '{';
					}
				}
			}
		}

		if (line == 14) {
			// pad the value to 25 columns
			if (value.size() < 25) {
				value.insert(value.end(), 25 - value.size(), ' ');
			}
		}

		Screen::getInstance()->drawLine(0, line - 1, value);
	}

	if (dataref == "FJCC/UFMC/Exec_Light_on") {
		LEDs::getInstance()->setLED(LEDs::LED_EXEC, value == "1");
	}

	else if (dataref == "FJCC/UFMC/Offset_on") {
		LEDs::getInstance()->setLED(LEDs::LED_OFST, value == "1");

	}
}

