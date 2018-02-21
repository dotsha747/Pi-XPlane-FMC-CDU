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

#ifndef PIXPLANEFMCCDU_SRC_ABSTRACTFMC_H_
#define PIXPLANEFMCCDU_SRC_ABSTRACTFMC_H_

#include <string>

class AbstractFMC {
public:
	AbstractFMC() {};
	virtual ~AbstractFMC() {};

	/** @brief returns name of FMC plugin.
	 *
	 * 	For "actual" FMCs, the name should be 4 characters, as it
	 * 	needs to fit on the RSKs on the Select screen. This name
	 * 	is also used as the index into FMCManager's actualFMCs
	 * 	map, to get to the actual FMC instances.
	 *
	 */
	virtual std::string getName() = 0;

	// called whenever ExtPlane connection established. Doesn't mean that
	// the current aircraft is using this FMC. FMC Plugins should subscribe
	// to whatever datarefs they need to determine that they are active.

	virtual void init () =0;

	/** @brief de-initialize the fmc.
	 *
	 * This gets called whenever an FMC is to be removed from the "current"
	 * screen. Triggered from setCurrentFMC().
	 *
	 * */

	virtual void deInit () = 0;


	/** @brief subscribe to datarefs
	 *
	 * 	all actual FMCs should subscribe to their datarefs here.
	 * 	It should be called from the actual FMC's init(), but may
	 * 	also be called from the FMCManager's onExtPlaneConnect()
	 * 	if the connection is dropped and reestablished.
	 *
	 */

	virtual void subscribeDataRefs () {};

	/** @brief called when we receive a float dataref from the server.*/

	virtual void receiveDataRef(std::string type, std::string dataref, std::string value) {};


	// these are called ONLY if FMCList thinks this FMC is currently active.
	// FMC's should call FMCList::setCurrentFMC if they think they are the
	// currently active one ... which essentially mean they are taking over
	// all keypress event processing.

	virtual void keyPressEvent (int row, int col) = 0;
	virtual void keyReleaseEvent (int row, int col) = 0;
};

#endif /* PIXPLANEFMCCDU_SRC_ABSTRACTFMC_H_ */
