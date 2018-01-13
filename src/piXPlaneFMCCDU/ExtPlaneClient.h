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
#include <functional>

#include "TCPClient.h"

class ExtPlaneClient: public TCPClient {
private:
	static ExtPlaneClient * instance;

	bool isRunning = false;
	int stopRequested = false;

	// callback hooks
	std::function <void()> onConnectCallback;
	std::function <void()> onDisconnectCallback;
	std::function <void (std::string, std::string, std::string)> receivedDataCallback;

public:
	ExtPlaneClient (
			std::string host,
			int port,
			std::function <void()> _onConnect,
			std::function <void()> _onDisconnect,
			std::function <void (std::string, std::string, std::string)> _receivedData
	);

	virtual ~ExtPlaneClient();

	void init();
	void launchThread();

	//virtual void mainLoop(int * exitFlag);

	void sendLine(std::string line);
	virtual void initConnection(time_t time);
	virtual void dropConnection(time_t time);
	void goLoop();
	virtual void processLine(time_t time, std::string line);
	virtual void processResponse(time_t time, std::string type,
			std::string dataref, std::string value);
protected:

};

#endif /* PIXPLANEFMCCDU_SRC_EXTPLANECLIENT_H_ */
