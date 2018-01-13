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
#include <unistd.h>

#include "ExtPlaneClient.h"
#include "Base64.h"
#include "Screen.h"


// initialize statics
ExtPlaneClient * ExtPlaneClient::instance = NULL;

using namespace std;

ExtPlaneClient::ExtPlaneClient (
		std::string host,
		int port,
		std::function <void()> _onConnect,
		std::function <void()> _onDisconnect,
		std::function <void (std::string, std::string, std::string)> _receivedData
) {

	// register our host (as the only one)
	ostringstream buf;
	buf << host << ":" << port;
	addHost(buf.str());

	// record the callback hooks
	onConnectCallback = _onConnect;
	onDisconnectCallback = _onDisconnect;
	receivedDataCallback = _receivedData;

	debug = 1;

	std::thread t(&ExtPlaneClient::goLoop, this);
	t.detach();

}

ExtPlaneClient::~ExtPlaneClient() {

	time_t nowTime = time(NULL);

	// this triggers the thread's mainloop to quit
	stopRequested = true;

	while (isRunning && time(NULL) < nowTime + 5) {
		usleep(100000); // 10ms
	}

	if (isRunning) {
		syslog (LOG_ERR, "ExtPlane mainLoop thread failed to stop within 5 seconds");
	}
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

void ExtPlaneClient::dropConnection(time_t time) {
	TCPClient::dropConnection(time);

	// call the callback hook
	onDisconnectCallback();
}

void ExtPlaneClient::processLine(time_t time, std::string line) {

	if (line.empty()) {
		return;
	}

	cerr << "RX " << line.size() << " [" << line << "]" << endl;

	// u{type} {dataref} {value}

	if (line == "EXTPLANE 1") {

		// call the callback hook
		onConnectCallback();

		// ask for aircraft typesa nd FMC flags
		/*
		sendLine("sub sim/aircraft/view/acf_descrip");
		sendLine("sub FJCC/UFMC/PRESENT");
		sendLine("sub SSG/UFMC/PRESENT");
		sendLine("sub xfmc/Status"); // XFMC active 0=off 1=on (bit0)
		*/

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

void ExtPlaneClient::processResponse(time_t time, std::string type,
		std::string dataref, std::string value) {

	// cerr << "Received Type:[" << type << "] dataref:[" << dataref << "] value:[" << value << "]" << endl;
	// cerr << "Decoded:[" << Base64::decode (value) << "]" << endl;

	// if it is base64 encoded, we decode it here.
	if (type == "ub") {
		value = Base64::decode (value);
	}

	receivedDataCallback (type, dataref, value);

}
