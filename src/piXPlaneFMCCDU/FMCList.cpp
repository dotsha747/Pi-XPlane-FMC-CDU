/*
 * FMCList.cpp
 *
 *  Created on: May 15, 2017
 *      Author: shahada
 */

#include <iostream>

#include "Base64.h"
#include "FMCList.h"

#include "ZeroFMC.h"
#include "X737FMC.h"
#include "ZiboFMC.h"

using namespace std;

// initialize statics
FMCList * FMCList::instance = NULL;




FMCList::FMCList() {

	cerr << "In FMCList constructor" << endl;
	fmcs.push_back(new ZeroFMC());
	currentFMC = fmcs[0];
	fmcs.push_back(new X737FMC());
	fmcs.push_back(new ZiboFMC());

}



void FMCList::init () {

	cerr << "In FMCList::Init" << endl;
	for (auto fmc : fmcs) {
		cerr << "  Calling " << fmc->getName() << "::Init" << endl;
		fmc->init();
	}

}


void FMCList::deinit () {

}


void FMCList::keyPressEvent (int row, int col) {

	cerr << "FMCList::keyPressEvent (" << row << ", " << col << ")" << endl;

	currentFMC->keyPressEvent(row, col);

}


void FMCList::keyReleaseEvent (int row, int col) {

	currentFMC->keyReleaseEvent(row, col);
}


void FMCList::receiveData (time_t time, std::string type, std::string dataref, std::string value) {

	// we have an aircraft change
	if (dataref == "sim/aircraft/view/acf_descrip") {
		if (!now_acf.empty()) {
			// deinit if there is an existing fmc
			deinit();
		}
		now_acf = Base64::decode(value);
		cerr << "acf_descrip is " << now_acf << endl;
		init();
	}

	for (auto fmc : fmcs) {
		fmc->receiveData(time, type, dataref, value);
	}
}


void FMCList::setCurrentFMC(AbstractFMC * fmc) {

	if (currentFMC != NULL) {
		deinit();
		currentFMC = NULL;
	}

	if (fmc != NULL) {
		currentFMC = fmc;
		fmc->init();
	}

}
