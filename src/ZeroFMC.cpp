/*
 * ZeroFMC.cpp
 *
 *  Created on: May 31, 2017
 *      Author: shahada
 */

#include "Screen.h"
#include "ZeroFMC.h"

#include <iostream>

using namespace std;

ZeroFMC::ZeroFMC() {


}

ZeroFMC::~ZeroFMC() {
	// TODO Auto-generated destructor stub
}


void ZeroFMC::init() {

	cerr << "In ZeroFMC::init" << endl;
	Screen::getInstance()->clear();
	Screen::getInstance()->queueLineUpdate(0, 0, "  Remote737 FMC CDU {V2}");
	Screen::getInstance()->queueLineUpdate(1, 0, "Status");
	Screen::getInstance()->queueLineUpdate(2, 0, "Connecting...");


}



std::string ZeroFMC::getName() {
	return "ZeroFMC";
}


void ZeroFMC::keyPressEvent (int row, int col) {

}


void ZeroFMC::keyReleaseEvent (int row, int col) {

}


void ZeroFMC::receiveData (time_t time, std::string type, std::string dataref, std::string value) {

}


