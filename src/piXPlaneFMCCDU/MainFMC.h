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

#ifndef PIXPLANEFMCCDU_MAINFMC_H_
#define PIXPLANEFMCCDU_MAINFMC_H_

#include <vector>
#include <XPlaneBeaconListener.h>
#include "AbstractFMC.h"

class MainFMC: public AbstractFMC {
protected:
	int numCols;
	int page;

	// @brief list discovered servers
	std::vector<XPlaneBeaconListener::XPlaneServer> servers;

	// @brief current server
	int nowServer;

	// @brief current FMC
	int nowFMC;

	std::vector<std::string> detectedFMCs;

public:

	virtual std::string getName() {
			return "MainFMC";
		}

	MainFMC();
	virtual ~MainFMC();

	virtual void init();
	virtual void deInit();
	virtual void receiveData(time_t time, std::string type, std::string dataref,
			std::string value);
	virtual void keyPressEvent(int row, int col);
	virtual void keyReleaseEvent(int row, int col);

	virtual void drawCenteredText(int line, std::string text);


	/* @brief refresh the display of the SelectFMC */
	void refreshDisplay ();

	/* @brief Update server availability
	 * Called from FMCManager whenever a XPlane server appears or disappears.
	 */

	void XPlaneBeaconListenerHandler (XPlaneBeaconListener::XPlaneServer server, bool exists);

	/* @brief x-plane disconnected
	 *
	 */

	void onExtPlaneDisconnect ();

	/** @brief add to the list of discovered FMCs.
	 *
	 * 	This is called by FMC Manager.
	 */

	void onDetectFMC (std::string fmc, bool present);
};

#endif /* PIXPLANEFMCCDU_MAINFMC_H_ */
