/*
 * XPlaneNetworkCLient.h
 *
 *  Created on: Jan 2, 2018
 *      Author: shahada
 */

#ifndef XPLANENETWORKCLIENT_H_
#define XPLANENETWORKCLIENT_H_


#include <XPlaneBeaconListener.h>

class XPlaneNetworkClient {
private:
	XPlaneNetworkClient () {}; // private so it cannot be called
	XPlaneNetworkClient (XPlaneNetworkClient const &) {}; // private so it cannot be called
	XPlaneNetworkClient & operator= (XPlaneNetworkClient const &) { abort();}; // assignment operator is private

	static XPlaneNetworkClient * instance;

	bool isRunning = false;
	int stopRequested = false;
	bool isConnected = false;

public:

	static XPlaneNetworkClient * getInstance() {
		if (!instance) {
			instance = new XPlaneNetworkClient ();
		}
		return instance;
	}

	virtual ~XPlaneNetworkClient();

	void init ();
	void launchThread ();
	void mainLoop ();


	void receiverBeaconCallback (XPlaneBeaconListener::XPlaneServer, bool exists);

	void connected ();
	void disconnected ();
	void receivedDataRef ();
};

#endif /* XPLANENETWORKCLIENT_H_ */
