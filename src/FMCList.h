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
	std::vector<AbstractFMC *> fmcs;

public:
	static FMCList * getInstance() {
		if (!instance) {
			instance = new FMCList();
		}
		return instance;
	}

	virtual ~FMCList() {};

	virtual void init ();

	virtual void keyPressEvent (int row, int col);
	virtual void keyReleaseEvent (int row, int col);
	virtual void receiveData (time_t time, std::string type, std::string dataref, std::string value);



};

#endif /* FMCLIST_H_ */
