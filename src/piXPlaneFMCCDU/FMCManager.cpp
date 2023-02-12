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

#include <sstream>
#include <unistd.h>
#include <functional>
#include <XPlaneBeaconListener.h>

#include "FMCManager.h"

#include "Screen.h"
#include "Version.h"

#include "ZiboFMC.h"
#include "X737FMC.h"
#include "XfmcFMC.h"

using namespace std;

// initialize statics
FMCManager * FMCManager::instance = NULL;

FMCManager::FMCManager() {

	// nobody is running yet
	currentFMC = NULL;
	xplaneConnection = NULL;

	splashFMC = new SplashFMC();
	mainFMC = new MainFMC();

	timeStarted = time(NULL);

	gotoSplashFMC();

	// create all the actual FMCs
	actualFMCs.insert(pair<std::string, AbstractFMC *> ("ZIB0", new ZiboFMC(true)));
	actualFMCs.insert(pair<std::string, AbstractFMC *> ("ZIB1", new ZiboFMC(false)));
	actualFMCs.insert(pair<std::string, AbstractFMC *> ("X737", new X737FMC()));
	actualFMCs.insert(pair<std::string, AbstractFMC *> ("XFMC", new XfmcFMC()));


}


FMCManager::~FMCManager() {
	// TODO Auto-generated destructor stub
}

void FMCManager::setCurrentFMC(AbstractFMC * fmc) {

	if (currentFMC != NULL) {
		currentFMC->deInit();
	}

	syslog(LOG_INFO, "Switching to %s", fmc->getName().c_str());

	currentFMC = fmc;

	currentFMC->init();

}

void FMCManager::keyPressEvent(int row, int col) {
	currentFMC->keyPressEvent(row, col);

	// CLR key pressed, record the time
	if (row == 1 && col == 8) {
		clearKeyPressTime = time(NULL);
	} else {
		clearKeyPressTime = 0;
	}
}

void FMCManager::keyReleaseEvent(int row, int col) {

	currentFMC->keyReleaseEvent(row, col);

	clearKeyPressTime = 0;
}

void FMCManager::rotaryEncTurnEvent(int count, int delta){
	currentFMC->rotaryEncTurnEvent(count, delta);
}
void FMCManager::tick() {

	time_t nowTime = time(NULL);

	// we're only ever in splashFMC on startup, so this should
	// happen only once.
	if (currentFMC == splashFMC && nowTime > timeStarted +4) {
		gotoMainFMC();
		// initialize X-Plane Beacon Listener
		XPlaneBeaconListener::getInstance()->registerNotificationCallback(
				std::bind(&FMCManager::XPlaneBeaconListener, this,
						std::placeholders::_1, std::placeholders::_2));
	}

	// if clear key pressed for more than 2 seconds
	if (clearKeyPressTime != 0 && clearKeyPressTime + 2 < nowTime) {
		clearKeyPressTime = 0;
			gotoMainFMC();
	}
}



void FMCManager::XPlaneBeaconListener(XPlaneBeaconListener::XPlaneServer server,
		bool exists) {

	// tell SelectFMC to update its server list.
	mainFMC->XPlaneBeaconListenerHandler(server, exists);

	syslog(LOG_INFO, "Beacon received %s with state %s",
			server.toString().c_str(), exists ? "online" : "offline");
}





void FMCManager::connectToServer(std::string host, int port) {

	ostringstream buf;
	buf << host << ":" << port;
	if (currentConnection != buf.str()) {
		if (xplaneConnection != NULL) {
			// this should call the destructor, which takes care of
			// shutting down threads and stuff.
			syslog (LOG_INFO, "Disconnecting from %s", currentConnection.c_str());
			delete xplaneConnection;

		}
	}
	currentConnection = buf.str();
	syslog (LOG_INFO, "Connecting to %s", currentConnection.c_str());
	xplaneConnection = new XPlaneExtPlaneClient::ExtPlaneClient (host, port,
			std::bind (&FMCManager::onExtPlaneConnect, this),
			std::bind (&FMCManager::onExtPlaneDisconnect, this),
			std::bind (&FMCManager::receiveDataFromServer, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
	);

}





void FMCManager::receiveDataFromServer(std::string type, std::string dataref, std::string value) {


	// aircraft type has changed.
	if (type=="ub" && dataref=="sim/aircraft/view/acf_descrip") {
		syslog (LOG_INFO, "acf_descrip [%s]", value.c_str());

		// Zibo
		if (value == "Boeing 737-800X") {
			mainFMC->onDetectFMC ("ZIB0", true);
			mainFMC->onDetectFMC ("ZIB1", true);
		} else {
			mainFMC->onDetectFMC ("Zib0", false);
			mainFMC->onDetectFMC ("Zib1", false);
		}

		setCurrentFMC (mainFMC);
		mainFMC->refreshDisplay();

	}

	if (type=="ui" && dataref=="FJCC/UFMC/x737FMC_Version") {
		syslog (LOG_INFO, "x737FMC found!");
		mainFMC->onDetectFMC ("X737", true);
		mainFMC->refreshDisplay();
	}

	if (type=="ui" && dataref=="xfmc/Status") {
		syslog (LOG_INFO, "XFMC found!");
		mainFMC->onDetectFMC ("XFMC", true);
		mainFMC->refreshDisplay();
	}

	currentFMC->receiveDataRef(type, dataref, value);
}




void FMCManager::subscribeDataRef (std::string dataref, float accuracy) {

	syslog (LOG_INFO, "SUB %s %f", dataref.c_str(), accuracy);

	if (xplaneConnection != NULL) {
		ostringstream buf;
		buf << "sub " << dataref;
		if (accuracy != 0) {
			buf << " " << accuracy;
		}
		xplaneConnection->sendLine (buf.str());
	}
}


void FMCManager::unsubscribeDataRef (std::string dataref) {
	syslog (LOG_INFO, "UNSUB %s", dataref.c_str());
	if (xplaneConnection != NULL) {
		ostringstream buf;
		buf << "unsub " << dataref;
		xplaneConnection->sendLine (buf.str());
	}
}


void FMCManager::setDataRef (std::string dataref, string value) {
	syslog (LOG_INFO, "SET %s %s", dataref.c_str(), value.c_str());
	if (xplaneConnection != NULL) {
		ostringstream buf;
		buf << "set " << dataref << " " << value;
		xplaneConnection->sendLine (buf.str());
	}
}



void FMCManager::sendCommand (std::string cmd) {
	syslog (LOG_INFO, "CMD [%s]", cmd.c_str());
		if (xplaneConnection != NULL) {
			ostringstream buf;
			buf << "cmd once " << cmd;
			xplaneConnection->sendLine (buf.str());
		}
}

void FMCManager::setCurrentFMC(std::string fmc) {

	if (actualFMCs.find(fmc) != actualFMCs.end()) {
		setCurrentFMC (actualFMCs.find(fmc)->second);
	} else {
		syslog (LOG_ERR, "can't find FMC %s", fmc.c_str());
	}
}


std::vector<std::string> FMCManager::getActualFMCList () {

	std::vector<std::string> ret;

	for (auto fmc : actualFMCs) {
		ret.push_back (fmc.first);
	}

	return ret;
}


void FMCManager::onExtPlaneConnect() {

	subscribeDataRef ("sim/aircraft/view/acf_descrip");
	subscribeDataRef ("FJCC/UFMC/x737FMC_Version");
	subscribeDataRef ("xfmc/Status"); // XFMC active 0=off 1=on (bit0)

	//xplaneConnection->sendLine("sub SSG/UFMC/PRESENT");

}


void FMCManager::onExtPlaneDisconnect() {
	mainFMC->onExtPlaneDisconnect();
	setCurrentFMC (mainFMC);
}
