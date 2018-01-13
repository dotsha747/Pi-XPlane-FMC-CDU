/*
 * MainFMC.cpp
 *
 *  Created on: Jan 13, 2018
 *      Author: shahada
 */

#include <string>
#include <syslog.h>
#include <sstream>

#include "FMCManager.h"
#include "Screen.h"
#include "LEDs.h"

#include "MainFMC.h"

using namespace std;

MainFMC::MainFMC() {

	numCols = 24;
	page = 0;
	nowServer = -1;
	nowFMC = -1;

}

MainFMC::~MainFMC() {

}

void MainFMC::init() {
	Screen::getInstance()->initialize(numCols);
	refreshDisplay();
	LEDs::getInstance()->setAllLEDs(0);
}

void MainFMC::deInit() {

}

void MainFMC::XPlaneBeaconListenerHandler(
		XPlaneBeaconListener::XPlaneServer server, bool exists) {

	if (exists) {
		syslog(LOG_INFO, "ADDING %s", server.toString().c_str());
		servers.push_back(server);
	} else {
		syslog(LOG_INFO, "REMOVING %s", server.toString().c_str());
		int c = 0;
		auto i = servers.begin();
		while (i != servers.end()) {
			if (i->host == server.host) {
				i = servers.erase(i);
				if (c == nowServer) {
					nowServer = -1;
				}
			} else {
				++i;
			}
			c++;
		}
	}

	syslog(LOG_INFO, "selectFMC has %u servers", (unsigned int) servers.size());
	refreshDisplay();

}

void MainFMC::refreshDisplay() {

	Screen::getInstance()->clearScreen();

	drawCenteredText(0, "Pi-XPlane-FMC-CDU");

	if (servers.size() == 0) {
		Screen::getInstance()->drawLine(0, 3, "status");
		Screen::getInstance()->drawLine(0, 4, "Waiting for X-Plane");
	} else {

		// draw the first 5  servers of the current page
		for (int i = 0; i < 5; i++) {
			if ((page * 5) + i < servers.size()) {
				ostringstream serverName;
				if ((page * 5) + i == nowServer) {
					serverName << "*" << servers[(page * 5) + i].name;
				} else {
					serverName << "<" << servers[(page * 5) + i].name;
				}
				Screen::getInstance()->drawLine(0, (i*2) + 2, serverName.str());
			};
		}

	}

	// draw FMCs
	for (int i = 0; i < 5; i++) {
		if ((page * 5) + i < detectedFMCs.size()) {
			ostringstream buf;
			buf << detectedFMCs[(page * 5) + i];
			if ((page * 5) + i == nowFMC) {
				buf << "*";
			} else {
				buf << ">";
			}
			Screen::getInstance()->drawLine(19, (i*2) + 2, buf.str());
		}
	}

	if (nowServer != -1 && nowFMC != -1) {
		LEDs::getInstance()->setLED(LEDs::LED_EXEC, 1);
	} else {
		LEDs::getInstance()->setLED(LEDs::LED_EXEC, 0);
	}

	Screen::getInstance()->drawLine(0, 12, "<Exit          Shutdown>");

}

void MainFMC::receiveData(time_t time, std::string type, std::string dataref,
		std::string value) {

}

void MainFMC::keyPressEvent(int row, int col) {

	syslog(LOG_INFO, "selectFMC got keypress %d %d", row, col);

	int lsk = -1;
	int rsk = -1;

	// LSK1
	if (row == 1 && col == 1) {
		lsk = 0;
	}

	// LSK2
	else if (row == 2 && col == 1) {
		lsk = 1;
	}

	// LSK3
	else if (row == 3 && col == 1) {
		lsk = 2;
	}

	// LSK4
	else if (row == 4 && col == 1) {
		lsk = 3;
	}

	// LSK5
	else if (row == 5 && col == 1) {
		lsk = 4;
	}

	// RSK1
	else if (row == 7 && col == 1) {
		rsk = 0;
	}

	// RSK2
	else if (row == 8 && col == 1) {
		rsk = 1;
	}

	// RSK3
	else if (row == 1 && col == 2) {
		rsk = 2;
	}

	// RSK4
	else if (row == 2 && col == 2) {
		rsk = 3;
	}

	// LSK6, "Exit"
	else if (row == 6 && col == 1) {
		syslog(LOG_INFO, "EXIT requested");
		Screen::getInstance()->quitLoop();

	}

	// RSK6, "Shutdown"
	else if (row == 4 && col == 2) {
		syslog(LOG_INFO, "SHUTDOWN requested");
		Screen::getInstance()->quitLoop();
	}

	// EXEC
	//else if (nowFMC != "" && nowServer != "" && row == 7 && col == 3) {
	//
	//	FMCManager::getInstance()->setCurrentFMC(nowFMC);
	//}

	// done with our button choices

	//if (nowServer != preServer || nowFMC != preFMC) {
	//	refreshDisplay();
	//}

	// if a server was selected, open its connection now.
	//if (nowServer != preServer) {
	//	FMCManager::getInstance()->connectToServer(
	//			servers[nowServer].host,
	//			servers[nowServer].receivePort
	//			);
	//}

	// a server was selected
	if (lsk != -1) {
		int newServer = lsk + (page * 5);
		if (newServer < servers.size()) {
			nowServer = newServer;
			FMCManager::getInstance()->connectToServer(servers[nowServer].host,
					51000); // ExtPlane Port, not X-Plane.
		}
	}

	// a fmc was selected
	if (rsk != -1) {
		int newFMC = rsk + (page * 5);
		if (newFMC < detectedFMCs.size()) {
			nowFMC = newFMC;
		}
	}

	// EXEC
	if (nowServer != -1 && nowFMC != -1 && row == 7 && col == 3) {
		FMCManager::getInstance()->setCurrentFMC(detectedFMCs[nowFMC]);

	}

	refreshDisplay();

}

void MainFMC::keyReleaseEvent(int row, int col) {

}

void MainFMC::drawCenteredText(int line, std::string text) {

	int x = (numCols - text.length()) / 2;
	Screen::getInstance()->drawLine(x, line, text);

}

void MainFMC::onExtPlaneDisconnect() {

	nowServer = -1;
	detectedFMCs.erase(detectedFMCs.begin(), detectedFMCs.end());
}

void MainFMC::onDetectFMC(std::string fmcName, bool present) {

	bool found = false;

	auto i = detectedFMCs.begin();

	while (i != detectedFMCs.end()) {
		if (*i == fmcName) {
			found = true;
			if (!present) {
				i = detectedFMCs.erase(i);
			} else {
				i++;
			}
		} else {
			i++;
		}
	}

	if (present && !found) {
		detectedFMCs.push_back(fmcName);
	}
}
