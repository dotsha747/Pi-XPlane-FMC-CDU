/*
 * X737FMC.cpp
 *
 *  Created on: May 15, 2017
 *      Author: shahada
 */

#include <iostream>
#include <sstream>

#include "ExtPlaneClient.h"
#include "X737FMC.h"

using namespace std;


X737FMC::X737FMC() {

	// populate keyInfo
	keyInfo[1][1] = "FJCC/UFMC/LK1";	// sw-1
	keyInfo [2][1] = "FJCC/UFMC/LK2";	// sw-2
	keyInfo [3][1] = "FJCC/UFMC/LK3"; // sw-3
	keyInfo [4][1] = "FJCC/UFMC/LK4"; 	// sw-4
	keyInfo [5][1] = "FJCC/UFMC/LK5"; 	// sw-5
	keyInfo [6][1] = "FJCC/UFMC/LK6"; 	// sw-6
	keyInfo [7][1] = "FJCC/UFMC/RK1"; 	// sw-7
	keyInfo [8][1] = "FJCC/UFMC/RK2"; 	// sw-8
	keyInfo [1][2] = "FJCC/UFMC/RK3"; 	// sw-9
	keyInfo [2][2] = "FJCC/UFMC/RK4";	// sw-10
	keyInfo [3][2] = "FJCC/UFMC/RK5"; 	// sw-11
	keyInfo [4][2] = "FJCC/UFMC/RK6"; 	// sw-12
	keyInfo [5][2] = "FJCC/UFMC/INITREF"; 	// sw-13
	keyInfo [6][2] = "FJCC/UFMC/RTE";	// sw-14
	keyInfo [7][2] = "FJCC/UFMC/CLB_x737"; 	// sw-15
	keyInfo [8][2] = "FJCC/UFMC/CRZ_x737";	// sw-16
	keyInfo [1][3] = "FJCC/UFMC/DES";	// sw-17
	keyInfo [2][3] = "FJCC/UFMC/MENU";	// sw-18
	keyInfo [3][3] = "FJCC/UFMC/LEGS"; // sw-19
	keyInfo [4][3] = "FJCC/UFMC/DEPARR";	// sw-20
	keyInfo [5][3] = "FJCC/UFMC/HOLD";	// sw-21
	keyInfo [6][3] = "FJCC/UFMC/PROG";	// sw-22
	keyInfo [7][3] = "FJCC/UFMC/EXEC";	// sw-23
	keyInfo [8][3] = "FJCC/UFMC/N1"; 	// sw-24
	keyInfo [1][4] = "FJCC/UFMC/FIX";	// sw-25
	keyInfo [2][4] = "FJCC/UFMC/PREVPAGE";	// sw-26
	keyInfo [3][4] = "FJCC/UFMC/NEXTPAGE"; // sw-27
	keyInfo [4][4] = "FJCC/UFMC/A";	// sw-28
	keyInfo [5][4] = "FJCC/UFMC/B";	// sw-29
	keyInfo [6][4] = "FJCC/UFMC/C";	// sw-30
	keyInfo [7][4] = "FJCC/UFMC/D";	// sw-31
	keyInfo [8][4] = "FJCC/UFMC/E";	// sw32
	keyInfo [1][5] = "FJCC/UFMC/F";	// sw-33
	keyInfo [2][5] = "FJCC/UFMC/G";	// sw-34
	keyInfo [3][5] = "FJCC/UFMC/H";	// sw-35
	keyInfo [4][5] = "FJCC/UFMC/I";	// sw-36
	keyInfo [5][5] = "FJCC/UFMC/J";	// sw-37
	keyInfo [6][5] = "FJCC/UFMC/K";	// sw-38
	keyInfo [7][5] = "FJCC/UFMC/L";	// sw-39
	keyInfo [8][5] = "FJCC/UFMC/M";	// sw-40
	keyInfo [1][6] = "FJCC/UFMC/N";	// sw-41
	keyInfo [2][6] = "FJCC/UFMC/O";	// sw-42
	keyInfo [3][6] = "FJCC/UFMC/P";	// sw-43
	keyInfo [4][6] = "FJCC/UFMC/Q";	// sw-44
	keyInfo [5][6] = "FJCC/UFMC/R";	// sw-45
	keyInfo [6][6] = "FJCC/UFMC/S";	// sw-46
	keyInfo [7][6] = "FJCC/UFMC/T";	// sw-47
	keyInfo [8][6] = "FJCC/UFMC/U";	// sw-48
	keyInfo [1][7] = "FJCC/UFMC/V";	// sw-49
	keyInfo [2][7] = "FJCC/UFMC/W";	// sw-50
	keyInfo [3][7] = "FJCC/UFMC/X";	// sw-51
	keyInfo [4][7] = "FJCC/UFMC/Y";	// sw-52
	keyInfo [5][7] = "FJCC/UFMC/Z";	// sw-53
	keyInfo [6][7] = "FJCC/UFMC/espacio";	// sw-54
	keyInfo [7][7] = "FJCC/UFMC/DEL";		// sw-55
	keyInfo [8][7] = "FJCC/UFMC/barra";	// sw-56
	keyInfo [1][8] = "FJCC/UFMC/CLR";		// sw-57
	keyInfo [2][8] = "FJCC/UFMC/punto";	// sw-58
	keyInfo [3][8] = "FJCC/UFMC/0";	// sw-59
	keyInfo [4][8] = "FJCC/UFMC/menos";// sw-60
	keyInfo [5][8] = "FJCC/UFMC/7"; 	// sw-61
	keyInfo [6][8] = "FJCC/UFMC/8";	// sw-62
	keyInfo [7][8] = "FJCC/UFMC/9";	// sw-63
	keyInfo [8][8] = "FJCC/UFMC/4";	// sw-64
	keyInfo [1][9] = "FJCC/UFMC/5";	// sw-65
	keyInfo [2][9] = "FJCC/UFMC/6";	// sw-66
	keyInfo [3][9] = "FJCC/UFMC/1";	// sw-67
	keyInfo [4][9] = "FJCC/UFMC/2";	// sw-68
	keyInfo [5][9] = "FJCC/UFMC/3";	// sw-69


}

X737FMC::~X737FMC() {
	// TODO Auto-generated destructor stub
}




std::string X737FMC::getName() {
	return "x737FMC";
}

void X737FMC::init () {

	// subscribe to all the keypad datarefs so we can set them later.

	for (auto rowV : keyInfo) {
		for (auto colV : rowV.second) {

			ostringstream buf;
			buf << "sub " << colV.second;
			ExtPlaneClient::getInstance()->sendLine(buf.str());
		}
	}

	// Subscribe to all the screen lines of the x737fmc.

	for (int line = 0; line <= 14; line++) {
		ostringstream buf;
		buf << "sub FJCC/UFMC/LINE_" << line;
		ExtPlaneClient::getInstance()->sendLine(buf.str());
	}

}


void X737FMC::deinit() {

}


void X737FMC::keyPressEvent (int row, int col) {

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

	cerr << "x737fmc keypress on r=" << rowV->first << " c=" << colV->first << " \"" << colV->second << "\"" << endl;

	ostringstream buf;
	buf << "set " << colV->second << " 1";
	ExtPlaneClient::getInstance()->sendLine(buf.str());
}



void X737FMC::keyReleaseEvent (int row, int col) {

	// x737 only deals with keyPressEvents, so do nothing.
}


void X737FMC::receiveData (time_t time, std::string type, std::string dataref, std::string value) {

	cerr << "X737fmc got [" << dataref << "|" << value << "]" << endl;
}
