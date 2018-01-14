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

	bool inLegsPage;

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

	virtual void subscribeDataRefs();

	/** @brief de-initialize the fmc.
	 *
	 * This gets called whenever an FMC is to be removed from the "current"
	 * screen. Triggered from setCurrentFMC().
	 *
	 * */

	virtual void deInit ();

	/** @brief called when we receive a float dataref from the server.*/

	virtual void receiveDataRef(std::string type, std::string dataref, std::string value);

	// @brief called when keypress detected. Translates to which key was pressed and sends command to ExtPlane to trigger the keypress.
	virtual void keyPressEvent (int row, int col);

	// @brief called when key release detected. Does nothing in x737FMC.
	virtual void keyReleaseEvent (int row, int col);



};

#endif /* PIXPLANEFMCCDU_SRC_X737FMC_H_ */
