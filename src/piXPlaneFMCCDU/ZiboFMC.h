/*
 This file is part of Pi-XPlane-FMC-CDU
 A Raspberry Pi-based External FMC for XPlane

 Copyright (C) 2017-2018 shahada abubakar
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


#ifndef PIXPLANEFMCCDU_SRC_ZIBOFMC_H_
#define PIXPLANEFMCCDU_SRC_ZIBOFMC_H_

#include <map>
#include <string>
#include <XPlaneUDPClient.h>

#include "AbstractFMC.h"

class ZiboFMC: public AbstractFMC {
protected:

	std::string side; // 0 = pilot, 1=first officer
	std::map<int, std::map<int, std::string>> keyInfo;

public:
	ZiboFMC(bool isCaptain);
	virtual ~ZiboFMC();

	virtual std::string getName();
	virtual void init();
	virtual void deInit();

	virtual void subscribeDataRefs();

	virtual void subscribe(std::string dataref);
	virtual void unsubscribe (std::string dataref);

	virtual void keyPressEvent(int, int);
	virtual void keyReleaseEvent(int, int);


	virtual void receiveDataRef(std::string, std::string dataref, std::string value);



};

#endif /* PIXPLANEFMCCDU_SRC_ZIBOFMC_H_ */
