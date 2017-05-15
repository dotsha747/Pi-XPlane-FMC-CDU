/*
 * FMCList.h
 *
 *  Created on: May 15, 2017
 *      Author: shahada
 */

#ifndef FMCLIST_H_
#define FMCLIST_H_

#include <stdlib.h>
#include <vector>

#include "AbstractFMC.h"

/** @brief Manage a List of FMC Modules
 *
 * To be able to support multiple FMCs, this module keeps a list of their implementation classes.
 *
 * Currently only one FMC is supported which is X737FMC.
 *
 * In the future, there will be an option to select which FMC you wish to use. This FMC will be
 * the "current" FMC.
 *
 * .
 *
 * This module receives all the datarefs sent by the ExtPlane server that are currently subscribed
 * to. It relays these to all the FMCs.
 *
 */

class FMCList {
private:
	FMCList(); // private so it cannot be called
	FMCList(FMCList const &) {}; // private so it cannot be called
	FMCList & operator=(FMCList const &) {
			abort();
	}; // assignment operator is private

	int currentFMC = 0;


protected:
	static FMCList * instance;

	// @brief holds the list of FMC implementations.
	std::vector<AbstractFMC *> fmcs;

public:

	// @brief return pointer to the singleton instance.
	static FMCList * getInstance() {
		if (!instance) {
			instance = new FMCList();
		}
		return instance;
	}

	virtual ~FMCList() {};

	// @brief initialize, called whenever a TCP session to an ExtPlane server is established.
	// It calls each FMC's own init method. This is usually used to subscribed to specific
	// datarefs needed by those FMCs' implementations.
	virtual void init ();

	// @brief called by the KeypadScanner whenever a keypress event is detected.
	// This calls the current FMC's keyPressEvent method.
	virtual void keyPressEvent (int row, int col);

	// @brief called by the KeypadScanner whenever a keypress release is detected.
	// This calls the current FMC's keyReleaseEvent method.
	virtual void keyReleaseEvent (int row, int col);

	// @brief called by ExtPlaneClient whenever a subscribed dataref changes value.
	// This calls all the FMC's receiveData method.
	virtual void receiveData (time_t time, std::string type, std::string dataref, std::string value);



};

#endif /* FMCLIST_H_ */
