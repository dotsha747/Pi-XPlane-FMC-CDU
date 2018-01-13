/*
 * MainFMC.h
 *
 *  Created on: Jan 13, 2018
 *      Author: shahada
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
