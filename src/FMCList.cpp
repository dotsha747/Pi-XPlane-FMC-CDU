/*
 * FMCList.cpp
 *
 *  Created on: May 15, 2017
 *      Author: shahada
 */

#include <iostream>

#include "FMCList.h"

#include "X737FMC.h"

using namespace std;

// initialize statics
FMCList * FMCList::instance = NULL;




FMCList::FMCList() {

	cerr << "In FMCList constructor" << endl;
	fmcs.push_back(new X737FMC());

}



void FMCList::init () {

	for (auto fmc : fmcs) {
		fmc->init();
	}


}


void FMCList::keyPressEvent (int row, int col) {

	fmcs[currentFMC]->keyPressEvent(row, col);

}


void FMCList::keyReleaseEvent (int row, int col) {

	fmcs[currentFMC]->keyReleaseEvent(row, col);
}


void FMCList::receiveData (time_t time, std::string type, std::string dataref, std::string value) {

	for (auto fmc : fmcs) {
		fmc->receiveData(time, type, dataref, value);
	}
}
