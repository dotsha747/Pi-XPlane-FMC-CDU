/*
 * AbstractFMS.h
 *
 *  Created on: May 15, 2017
 *      Author: shahada
 */

#ifndef PIXPLANEFMCCDU_SRC_ABSTRACTFMC_H_
#define PIXPLANEFMCCDU_SRC_ABSTRACTFMC_H_

#include <string>

class AbstractFMC {
public:
	AbstractFMC() {};
	virtual ~AbstractFMC() {};

	// returns name of FMC plugin
	virtual std::string getName() = 0;

	// called whenever ExtPlane connection established. Doesn't mean that
	// the current aircraft is using this FMC. FMC Plugins should subscribe
	// to whatever datarefs they need to determine that they are active.

	virtual void init () =0;

	// passed all datarefs received frm ExtPlane. This would include common
	// ones subscribed by ExtPlaneClient such as acf_descrip, and those
	// requested by plugins during init. Expect to get stuff for other FMCs
	// in your FMC too, which should be ignored. FMCs should monitor
	// acf_descrip (and other per-FMC datarefs subscribed to in init) to see if
	// they are active. Only if they are active (and they should keep track of
	// this state) should they subscribe to FMC specific datarefs and process
	// them (to keep the load light when they are not active).

	virtual void receiveData (time_t time, std::string type, std::string dataref, std::string value) = 0;

	// these are called ONLY if FMCList thinks this FMC is currently active.
	// FMC's should call FMCList::setCurrentFMC if they think they are the
	// currently active one ... which essentially mean they are taking over
	// all keypress event processing.

	virtual void keyPressEvent (int row, int col) = 0;
	virtual void keyReleaseEvent (int row, int col) = 0;
};

#endif /* PIXPLANEFMCCDU_SRC_ABSTRACTFMC_H_ */
