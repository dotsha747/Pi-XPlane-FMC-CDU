/*
 * FMCManager.h
 *
 *  Created on: Jan 7, 2018
 *      Author: shahada
 */

#ifndef FMCMANAGER_H_
#define FMCMANAGER_H_

#include <stdlib.h>
#include <time.h>


#include <XPlaneUDPClient.h>
#include <syslog.h>

#include "AbstractFMC.h"
#include "SplashFMC.h"
#include "MainFMC.h"

#include "ExtPlaneClient.h"

/** @brief Manages "current" screen by channelling data and keypresses to a FMC Handler
 *
 */

class FMCManager {
private:
	// private so it cannot be called publicly
	FMCManager();

	// private so it cannot be called publicly
	FMCManager(FMCManager const &) {};


	// assignment operator is private too
	FMCManager & operator =(FMCManager const &) {
		abort();
	}

	AbstractFMC * currentFMC;

	// here are our utility FMCs
	SplashFMC * splashFMC;
	MainFMC * mainFMC;


	// actual FMCS go here
	std::map<std::string, AbstractFMC *> actualFMCs;

	// hack to keep the last host for initHost()
	std::string hackHost;
	int hackPort;

protected:
	static FMCManager * instance;

	/** @brief time when the FMCManager was created */
	time_t timeStarted;

	virtual ~FMCManager();

	void drawCenteredText(int line, std::string text);
	void splash();
	void setCurrentFMC(AbstractFMC * fmc);

	/** @brief tracker for current server we're connected to.
	 *
	 * 	a contraction of host + ":" + port. We just use this
	 * 	to see if a connection request is to a *different*
	 * 	server, in which case we shutdown the old connection
	 * 	first.
	 *
	 */
	std::string currentConnection;

	/** @brief our "connection" to the server
	 *
	 */
	ExtPlaneClient * xplaneConnection;

	/** track time when CLR was pressed.
	 * 	Used to trigger going to Select screen if held
	 * 	for more than 2 seconds.
	 */
	time_t clearKeyPressTime = 0;

public:
	// @brief return pointer to the singleton instance
	static FMCManager * getInstance() {
		if (!instance) {
			instance = new FMCManager();
		};
		return instance;
	}

	void keyPressEvent(int row, int col);
	void keyReleaseEvent(int row, int col);

	/** @brief called once every second by Screen's mainloop
	 *
	 */

	void tick();

	void gotoSplashFMC() {
		setCurrentFMC(splashFMC);
	}

	void gotoMainFMC() {
		setCurrentFMC(mainFMC);
	}

	void XPlaneBeaconListener(XPlaneBeaconListener::XPlaneServer server,
			bool exists);


	/** @brief connect to the server
	 *
	 *	Well not really a "connection" since we use the UDP interface,
	 *	but this at least creates the XPlaneUDPClient instance. It's
	 *	actually possible to connect to a server that isn't there,
	 *	so ...
	 *
	 *	We track the "current" connection server by name, so if it
	 *	if the connection is for a different server, the the old
	 *	connection is shutdown first.
	 */

	void connectToServer(std::string host, int port);


	/** @brief callback from network client
	 *
	 * 	This is called whenever a dataref update is received from the client.
	 *
	 */
	void receiveDataFromServer(std::string type, std::string dataref, std::string value);




	/** @brief called when server connected.
	 *
	 *  We should subscribe to datarefs we need here.
	 */

	void onExtPlaneConnect ();


	/** @brief called when server disconnected.
	 *
	 */

	void onExtPlaneDisconnect ();


	/** @brief subscribe to a dataref from the current server
	 *
	 * @param dataref
	 */

	void subscribeDataRef (std::string dataref, float accuracy = 0);


	/** @brief unsubscribe from a dataref from the current server
	 *
	 * @param dataref
	 */

	void unsubscribeDataRef (std::string dataref);

	/** @brief set dataref on the server
	 *
	 */

	void setDataRef (std::string dataref, std::string value);

	/** @brief sets current FMC based on string
	 *
	 * 	Used to set an active FMC, by pressing EXEC in selectFMC.
	 * 	Looks up the instance by name in actualFMCs, then calls
	 * 	setCurrentFMC (AbstractFMC * fmc).
	 *
	 * @param fmc
	 */
	void setCurrentFMC(std::string fmc);


	/** @brief gets names of actual FMCs */

	std::vector<std::string> getActualFMCList ();

	/** @brief send CMD to current server */
	void sendCommand (std::string cmd);

};

#endif /* FMCMANAGER_H_ */

