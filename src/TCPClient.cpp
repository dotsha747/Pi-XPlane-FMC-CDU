/*
 * TCPClient.cpp
 *
 *  Created on: Apr 3, 2016
 *      Author: shahada
 */

#include <string>
#include <unistd.h>
#include <errno.h>
#include <sys/select.h>
#include <time.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <cstring>
#include <syslog.h>
#include <iostream>

#include "TCPClient.h"

using namespace std;

TCPClient::TCPClient() {

	host = "192.168.1.10";
	port = 51000;
	connectTimeout = 60;
	clientSock = -1;
	connectState = CONN_DISCONNECTED;
	wantWrite = false;
	wantRead = false;
	EOLString = "\n";
	lastDataReceivedTime = time(NULL);

}

TCPClient::~TCPClient() {
	// TODO Auto-generated destructor stub
}

void TCPClient::mainLoop() {

	int exitFlag = 0;

	cerr << "In TCPClient::mainLoop" << endl;

	time_t lastTick = time(NULL);

	while (!(exitFlag)) {

		// set select() bits
		fd_set read_fds, write_fds;
		FD_ZERO(&read_fds);
		FD_ZERO(&write_fds);

		if (wantRead) {
			FD_SET(clientSock, &read_fds);
		}

		if (wantWrite) {
			FD_SET(clientSock, &write_fds);
		}

		// figure out timeout till next second
		timeval timeout = { 0, 10000 }; // sleep for 10 m/s

		int ret;

		if ((ret = select(clientSock + 1, &read_fds, &write_fds, NULL, &timeout))
				== -1) {

			syslog(LOG_ERR, "select() error: %s", strerror(errno));
			continue;
		}

		// exit in last second?
		if (exitFlag) {
			break;
		}

		time_t nowTime = time(NULL);

		if (nowTime > lastTick) {
			// do per-second processing
			eventTick(nowTime);
		}

		// check for READ
		if (FD_ISSET(clientSock, &read_fds)) {
			eventRead(nowTime);
		}

		// check for WRITE
		if (FD_ISSET(clientSock, &write_fds)) {
			eventWrite(nowTime);
		}

	} // while not the end

	dropConnection(time(NULL));

}

void TCPClient::eventTick(time_t _time) {

	// deal with connection
	if (connectState == CONN_DISCONNECTED) {

		// don't bother if we have no hosts
		if (hostList.empty()) {
			syslog(LOG_ERR, "No server hosts defined!");
			sleep(5);
			return;
		}

		// figure out next host to try
		hostNow++;
		if (hostNow > hostList.size() - 1) {
			hostNow = 0;
		}

		// split host and port (if provided)
		host = hostList[hostNow];
		port = 51000;
		if (auto pos = host.find_first_of(':', 0) != string::npos) {
			cerr << "Extract port [" << host.substr(pos + 1) << "]" << endl;
			port = std::stoi(host.substr(pos + 1));
			cerr << "Extract host [" << host.substr(0, pos - 1) << "]" << endl;
			host = host.substr(0, pos - 1);
		}

		// get socket
		clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		// try to connect to socket
		struct sockaddr_in sa;
		struct hostent * h = gethostbyname(host.c_str());
		if (h != NULL) {
			memcpy((char *) &sa.sin_addr, (char *) h->h_addr, h->h_length);
			sa.sin_family = h->h_addrtype;
			sa.sin_port = htons (port);
		} else {
			syslog (LOG_ERR, "Unable to resolve %s:%d",
			host.c_str(), port);
			return;
		};
		memset(&sa.sin_zero, 0, sizeof(sa.sin_zero));

		// set socket to non-blocking mode
		if (fcntl(clientSock, F_SETFL, O_NONBLOCK) == -1) {
			syslog(LOG_WARNING,
					"Couldn't set O_NONBLOCK for socket %d to %s:%d",
					clientSock, host.c_str(), port);
			return;
		}

		// try to connect
		syslog(LOG_INFO, "Connecting to %s:%d", host.c_str(), port);
		int status = connect(clientSock, (sockaddr *) &sa, sizeof(sa));
		lastDataReceivedTime = _time;

		if (status == 0) {

			// immediate connection
			initConnection(_time);

		} else {

			// not connected, maybe later
			connectState = CONN_CONNECTING;
			syslog(LOG_INFO, "Waiting for connection to %s:%d on socket #%d",
					host.c_str(), port, clientSock);
			wantWrite = true;
		}

	} // MODE_DISCONNECTED

	else if (connectState == CONN_CONNECTED) {

		// output buffer too big
		if (outputBuffer.size() > 8192) {
			syslog(LOG_INFO, "Output buffer overflow on %s:%d", host.c_str(),
					port);
			dropConnection(_time);
		}
	} // MODE_CONNECTED

	else if (connectState == CONN_CONNECTING) {

		if (_time - lastDataReceivedTime > connectTimeout) {
			syslog(LOG_INFO, "Connect timeout with %s:%d", host.c_str(), port);
			dropConnection(_time);
		}

	}

}

void TCPClient::initConnection(time_t _time) {

	connectState = CONN_CONNECTED;

	wantRead = true;
	inputBuffer.clear();
	outputBuffer.clear();

	syslog(LOG_INFO, "Connection established to %s:%d on socket #%d",
			host.c_str(), port, clientSock);

}

void TCPClient::dropConnection(time_t _time) {

	if (connectState != CONN_DISCONNECTED) {
		wantRead = false;
		wantWrite = false;
		close(clientSock);

		inputBuffer.clear();
		outputBuffer.clear();

		connectState = CONN_DISCONNECTED;

	}
}

double TCPClient::getHighResolutionTime() {

	// Get time
	struct timeval tv;
	struct timezone tz;

	gettimeofday(&tv, &tz);
	return (double) tv.tv_sec + ((double) tv.tv_usec / 1000000);
}

void TCPClient::eventRead(time_t _time) {

	if (connectState != CONN_CONNECTED) {
		syslog(LOG_ERR, "Read event while not connected");
		return;
	}

	int bufsize = inputBuffer.length();
	char buf[1024];

	int len = read(clientSock, buf, 1024 - bufsize - 1);
	buf[len] = '\0';

	if (len == -1) {

		// read error
		syslog(LOG_ERR, "read error on socket #%d from %s:%d : %s", clientSock,
				host.c_str(), port, strerror(errno));
		dropConnection(_time);

	} else if (len == 0) {

		// end of file, on a socket?
		syslog(LOG_ERR, "end of input on socket #%d from %s:%d : %s",
				clientSock, host.c_str(), port, strerror(errno));
		dropConnection(_time);
	}

	inputBuffer += buf;

	processInput(_time);

}

void TCPClient::eventWrite(time_t _time) {

	if (connectState == CONN_DISCONNECTED) {
		return;
	}

	if (connectState == CONN_CONNECTING) {
		// either connected, or failed. Write 0 bytes, and see what happens.
		ssize_t c = send(clientSock, "", 0, 0);
		if (c == -1) {
			syslog(LOG_ERR, "Error connecting to %s:%d on socket #%d: %s",
					host.c_str(), port, clientSock, strerror(errno));
			sleep (1);
			dropConnection(_time);
		} else {
			initConnection(_time);
		}

		return;
	}

	// ExtPlane seems to look at only the first "line" in a packet.
	// Therefore, if there are multiple lines in our outputBuffer,
	// try to send them one at a time.

	size_t len = outputBuffer.find('\n');
	if (len == string::npos) {
		len = outputBuffer.length();
	} else {
		len += 1;
	}

	// write from buffer
	ssize_t c = send(clientSock, outputBuffer.c_str(), len, 0);

	if (c == -1) {
		syslog(LOG_ERR, "Failed on write to %s:%d #%d: %s", host.c_str(), port,
				clientSock, strerror(errno));
		dropConnection(_time);
	} else {
		cerr << "TX " << len << " [" << outputBuffer.substr(0, c) << "]"
				<< endl;
		// trim buffer
		outputBuffer = outputBuffer.substr(c);

		if (outputBuffer.empty()) {
			wantWrite = false;
		}
	}

}

void TCPClient::processInput(time_t _time) {

	// find "lines"

	std::string::size_type pos;
	while ((pos = inputBuffer.find(EOLString)) != std::string::npos) {

		// extract out a line to a separate string
		std::string line = inputBuffer.substr(0, pos);

		// process the line
		processLine(_time, line);

		// shift the input buffer
		inputBuffer.erase(0, pos + EOLString.size());

	}; // while
}

void TCPClient::processLine(time_t _time, std::string line) {

	cout << "TCPClient::processline GOT [" << line << "]" << endl;
}

void TCPClient::addHost(std::string _host) {

	hostList.push_back(_host);
}

unsigned int TCPClient::getHostCount() {

	return hostList.size();
}

void TCPClient::sendData(std::string data) {
	outputBuffer.append(data);
	if (!outputBuffer.empty()) {
		wantWrite = true;
	}
}
