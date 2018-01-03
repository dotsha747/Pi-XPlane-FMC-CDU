/*
 * ExtPlaneClient.h
 *
 *  Created on: May 1, 2017
 *      Author: shahada
 */

#ifndef PIXPLANEFMCCDU_SRC_EXTPLANECLIENT_H_
#define PIXPLANEFMCCDU_SRC_EXTPLANECLIENT_H_

#include <syslog.h>
#include <time.h>

#include "TCPClient.h"


class ExtPlaneClient : public TCPClient {
private:
	ExtPlaneClient() {}; // private so it cannot be called
	ExtPlaneClient(ExtPlaneClient const &) {}; // private so it cannot be called
	ExtPlaneClient & operator=(ExtPlaneClient const &) {
			abort();
	}; // assignment operator is private

	static ExtPlaneClient * instance;

	bool isRunning = false;
	int stopRequested = false;

public:
	static ExtPlaneClient * getInstance() {
		if (!instance) {
			instance = new ExtPlaneClient();
		}
		return instance;
	}

	virtual ~ExtPlaneClient();

	void init ();
	void launchThread ();

	//virtual void mainLoop(int * exitFlag);

	void sendLine (std::string line);
	virtual void initConnection (time_t time);
	virtual void dropConnection (time_t time);
	void goLoop ();
	virtual void processLine (time_t time, std::string line);
	virtual void processResponse (time_t time, std::string type, std::string dataref, std::string value);
protected:


};

#endif /* PIXPLANEFMCCDU_SRC_EXTPLANECLIENT_H_ */
