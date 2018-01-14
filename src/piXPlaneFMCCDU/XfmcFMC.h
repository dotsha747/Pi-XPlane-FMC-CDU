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

#ifndef PIXPLANEFMCCDU_XFMCFMC_H_
#define PIXPLANEFMCCDU_XFMCFMC_H_

#include "AbstractFMC.h"

class XfmcFMC: public AbstractFMC {
protected:

public:
	XfmcFMC();
	virtual ~XfmcFMC();

	virtual std::string getName() {
		return "XFMC";
	}

	virtual void init ();
	virtual void deInit ();
	virtual void receiveDataRef(std::string type, std::string dataref, std::string value);

	virtual void keyPressEvent (int row, int col);
	virtual void keyReleaseEvent (int row, int col);
};

#endif /* PIXPLANEFMCCDU_XFMCFMC_H_ */
