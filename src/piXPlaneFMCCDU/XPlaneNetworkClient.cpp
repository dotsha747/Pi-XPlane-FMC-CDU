/*
 * XPlaneNetworkCLient.cpp
 *
 *  Created on: Jan 2, 2018
 *      Author: shahada
 */

#include <syslog.h>
#include <thread>
#include <iostream>
#include <unistd.h>

#include <XPlaneUDPClient.h>


#include "XPlaneNetworkClient.h"

using namespace std;

// initialize statics
XPlaneNetworkClient * XPlaneNetworkClient::instance = NULL;

XPlaneNetworkClient::~XPlaneNetworkClient() {
	// TODO Auto-generated destructor stub
}

void XPlaneNetworkClient::init() {

	cerr << "In XPlaneNetworkClient::init" << endl;

	isRunning = false;
	stopRequested = false;

	cerr << "Finished XPlaneNetworkClient::init" << endl;
}

void XPlaneNetworkClient::launchThread() {

	cerr << "In XPlaneNetworkClient::launchThread" << endl;

	if (!isRunning) {
		std::thread t(&XPlaneNetworkClient::mainLoop, this);
		t.detach();
	}

	cerr << "Finished XPlaneNetworkClient::launchThread" << endl;
}

void XPlaneNetworkClient::mainLoop() {

	cerr << "In XPlaneNetworkClient::goLoop" << endl;

	// register a callback from the beacon listener
	XPlaneBeaconListener::getInstance()->registerNotificationCallback(
	      std::bind(&XPlaneNetworkClient::receiverBeaconCallback, this, std::placeholders::_1,
	          std::placeholders::_2));


	isRunning = true;
	time_t lastTick = time(NULL);

	while (!(stopRequested)) {

		cerr << "NewTick ..." << endl;
		sleep (1);
 	}

	cerr << "Finished XPlaneNetworkClient::mainLoop()" << endl;
}


void XPlaneNetworkClient::receiverBeaconCallback (XPlaneBeaconListener::XPlaneServer server, bool exists) {

	cerr << "Got beacon " << server.toString() << " exists " << exists << endl;

}


void XPlaneNetworkClient::connected () {};

void XPlaneNetworkClient::disconnected () {};

void XPlaneNetworkClient::receivedDataRef () {};
