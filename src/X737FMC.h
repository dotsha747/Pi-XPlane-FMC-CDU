/*
 * X737FMC.h
 *
 *  Created on: May 15, 2017
 *      Author: shahada
 */

#ifndef PIXPLANEFMCCDU_SRC_X737FMC_H_
#define PIXPLANEFMCCDU_SRC_X737FMC_H_

#include <map>
#include <vector>
#include <string>
#include "AbstractFMC.h"

// @brief driver for Javier Cortés x737FMC, for use with the EADT x737.
// Note for later: For SSG 748, replace FJCC with SSG in datarefs, i.e.
// SSG/UFMC/PRESENT.

class X737FMC: public AbstractFMC {
protected:
	std::map<int, std::map<int, std::string>> keyInfo;

public:

	// @brief constructor. Defines the mapping between keypad row/col combinations
	// and the datarefs that can be used to indicate a keypress/release in x737FMC.
	X737FMC();

	virtual ~X737FMC();

	// @brief returns the name of FMC i.e. "x737FMC"
	virtual std::string getName();

	// @brief called when connected to ExtPlane server, it subscribes to required datarefs.
	// includes datarefs for keypresses, screen lines, and LED indicators.
	virtual void init ();

	virtual void deinit ();

	// @brief called when keypress detected. Translates to which key was pressed and sends command to ExtPlane to trigger the keypress.
	virtual void keyPressEvent (int row, int col);

	// @brief called when key release detected. Does nothing in x737FMC.
	virtual void keyReleaseEvent (int row, int col);

	// @brief receives datarefs that have changed from ExtPlane.
	// TODO: *should* be used to update the screen.
	virtual void receiveData (time_t time, std::string type, std::string dataref, std::string value);

	// @brief subscribes to all datarefs
	virtual void subscribe (time_t time);

	// @brief unsubscribes to all datarefs
	virtual void unsubscribe (time_t time);
};

#endif /* PIXPLANEFMCCDU_SRC_X737FMC_H_ */
