/*
 This file is part of Pi-XPlane-FMC-CDU
 A Raspberry Pi-based External FMC for XPlane

 Copyright (C) 2017-2018 shahada abubakar
 <shahada@abubakar.net>

File contributed by Lefteris Kororos <lkororos@gmail.com>

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
#ifndef PIXPLANEFMCCDU_SRC_ROTARYENCSCANNER_H_
#define PIXPLANEFMCCDU_SRC_ROTARYENCSCANNER_H_

#include <syslog.h>
#include <vector>
#include <stdlib.h>

class RotaryEncScanner {
private:
    int count;
	RotaryEncScanner(); // private so it cannot be called

	RotaryEncScanner(RotaryEncScanner const &) {};

	RotaryEncScanner & operator=(RotaryEncScanner const &) {
		abort();
	}	 // assignment operator is private

protected:
	static RotaryEncScanner * instance;

    // ROTA and ROTB inputs
    std::vector<int> pinsAB = {
        28, // p.38 BCM. 20 ROTA
        25, // p.37 BCM. 26 ROTB
    };

	bool isRunning;
	bool stopRequested;

	unsigned int pressedRow;
	unsigned int pressedCol;
	bool somethingPressed;

public:

	static RotaryEncScanner * getInstance() {
		if (!instance) {
			syslog(LOG_INFO, "Intantiating RotaryEncScanner");
			instance = new RotaryEncScanner();
		}
		return instance;
	}

	void launchThread();
	void mainLoop();

	virtual ~RotaryEncScanner();

};

#endif /* PIXPLANEFMCCDU_SRC_ROTARYENCSCANNER_H_ */
