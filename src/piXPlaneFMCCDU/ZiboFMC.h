/*
 * ZiboFMC.h
 *
 *  Created on: Jul 26, 2017
 *      Author: shahada
 */


#ifndef PIXPLANEFMCCDU_SRC_ZIBOFMC_H_
#define PIXPLANEFMCCDU_SRC_ZIBOFMC_H_

#include <map>
#include <string>
#include <XPlaneUDPClient.h>

#include "AbstractFMC.h"

class ZiboFMC: public AbstractFMC {
protected:

	std::map<int, std::map<int, std::string>> keyInfo;

	XPlaneUDPClient * xplaneUDPClient;
public:
	ZiboFMC();
	virtual ~ZiboFMC();

	virtual std::string getName();
	virtual void init();
	virtual void initSetHost (std::string host, int port);
	virtual void deInit();

	virtual void subscribeDataRefs();

	virtual void keyPressEvent(int, int);
	virtual void keyReleaseEvent(int, int);

	virtual std::string mixLargeSmallLines(std::string large,
			std::string small);
	virtual void receiveDataRef(std::string, std::string dataref, std::string value);

	virtual void receiveUDPDataRefString (std::string dataref, std::string value);
	virtual void receiveUDPDataRefFloat (std::string dataref, float value);


};

#endif /* PIXPLANEFMCCDU_SRC_ZIBOFMC_H_ */
