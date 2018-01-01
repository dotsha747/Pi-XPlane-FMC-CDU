/*
 * ExtPlaneClient.cpp
 *
 *  Created on: May 1, 2017
 *      Author: shahada
 */

#include <syslog.h>
#include <thread>
#include <iostream>
#include <sstream>
#include <regex>

#include "ExtPlaneClient.h"
#include "Base64.h"
#include "Screen.h"
#include "FMCList.h"

// initialize statics
ExtPlaneClient * ExtPlaneClient::instance = NULL;

using namespace std;

ExtPlaneClient::~ExtPlaneClient() {
	// TODO Auto-generated destructor stub
}

void ExtPlaneClient::init() {

	cerr << "In ExtPlaneClient::init" << endl;

	if (hostList.empty()) {
		hostList.push_back("192.168.1.10");
	}

	isRunning = false;
	stopRequested = false;

	cerr << "Finished ExtPlaneClient::init" << endl;
}

void ExtPlaneClient::launchThread() {

	cerr << "In ExtPlaneClient::launchThread" << endl;
	if (!isRunning) {
		std::thread t(&ExtPlaneClient::goLoop, this);
		t.detach();
	}
	cerr << "Finished ExtPlaneClient::goLoop" << endl;

}


void ExtPlaneClient::goLoop() {

	cerr << "In ExPlaneClient::goLoop" << endl;
	TCPClient::mainLoop();
	cerr << "Finished ExtPlaneClient::goLoop" << endl;
}



void ExtPlaneClient::sendLine(std::string line) {
	outputBuffer += line;
	outputBuffer += "\n";
	wantWrite = true;
}

void ExtPlaneClient::initConnection(time_t time) {
	TCPClient::initConnection(time);
}


void ExtPlaneClient::dropConnection (time_t time) {
	TCPClient::dropConnection (time);
	// cerr << "Connection dropped" << endl;
}

void ExtPlaneClient::processLine(time_t time, std::string line) {

	if (line.empty()) {
		return;
	}

	cerr << "RX " << line.size() << " [" << line << "]" << endl;

	// u{type} {dataref} {value}

	if (line == "EXTPLANE 1") {

		// ask for aircraft types.
		sendLine("sub sim/aircraft/view/acf_descrip");
		sendLine("sub FJCC/UFMC/PRESENT");
		sendLine("sub SSG/UFMC/PRESENT");
		sendLine("sub xfmc/Status"); // XFMC active 0=off 1=on (bit0)

	}

	else if (line.at(0) == 'u') {

		size_t pos1 = line.find_first_of(' ', 0);
		if (pos1 == string::npos) {
			syslog(LOG_WARNING, "no initial space in [%s]", line.c_str());
		}

		size_t pos2 = line.find_first_of(' ', pos1 + 1);
		if (pos2 == string::npos) {
			syslog(LOG_WARNING, "no second space in [%s]", line.c_str());
		}

		processResponse(time, line.substr(0, pos1),
				line.substr(pos1 + 1, pos2 - pos1 - 1), line.substr(pos2 + 1));
	}
}



void ExtPlaneClient::processResponse (time_t time, std::string type, std::string dataref, std::string value) {

	// cerr << "Received Type:[" << type << "] dataref:[" << dataref << "] value:[" << value << "]" << endl;
	// cerr << "Decoded:[" << Base64::decode (value) << "]" << endl;

	// received data is sent to ALL FMC plugins.
	FMCList::getInstance()->receiveData(time, type, dataref, value);


}
