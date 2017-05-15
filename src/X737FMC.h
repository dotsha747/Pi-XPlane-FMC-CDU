/*
 * X737FMC.h
 *
 *  Created on: May 15, 2017
 *      Author: shahada
 */

#ifndef X737FMC_H_
#define X737FMC_H_

#include <map>
#include <vector>
#include <string>
#include "AbstractFMC.h"


class X737FMC: public AbstractFMC {
protected:
	std::map<int, std::map<int, std::string>> keyInfo;

public:
	X737FMC();
	virtual ~X737FMC();

	virtual std::string getName();
	virtual void init ();
	virtual void deinit ();

	virtual void keyPressEvent (int row, int col);
	virtual void keyReleaseEvent (int row, int col);
	virtual void receiveData (time_t time, std::string type, std::string dataref, std::string value);
};

#endif /* X737FMC_H_ */
