/*
 This file is part of TestRotaryEncoder
 A Raspberry Pi-based Demo of a dual-rotary encoder and X-Plane

 Copyright (C) 2018 shahada abubakar
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
