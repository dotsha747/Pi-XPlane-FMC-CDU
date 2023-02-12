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

#ifndef PIXPLANEFMCCDU_SRC_ARM_KEYPADSCANNER_H_
#define PIXPLANEFMCCDU_SRC_ARM_KEYPADSCANNER_H_

#include <syslog.h>
#include <vector>
#include <stdlib.h>

class KeypadScanner {
private:
	KeypadScanner(); // private so it cannot be called

	KeypadScanner(KeypadScanner const &) {};

	KeypadScanner & operator=(KeypadScanner const &) {
		abort();
	}	 // assignment operator is private

protected:
	static KeypadScanner * instance;

	// columns are outputs. All are set to HIGH except the column being scanned.
	std::vector<int> columnPins = { 15, // p.8  BCM.14 	BLACK UART TX
			16, // p.10 BCM.15 	WHITE UART RX
			1, 	// p.12 BCM.18 	GRAY  PWM0
			4, 	// p.16 BCM.23 	PURPLE
			5, 	// p.18 BCM.24 	BLUE
			6,	// p.22 BCM.25 	GREEN
			10,	// p.24	BCM.8	YELLOW CE0
			11,	// p.26 BCM.7	ORANGE CE1
			27	// p.36 BCM.16	RED
			};

	// 	// rows are inputs.
	std::vector<int> rowPins = { 29,	//	p.40 BCM.21	GRAY SCLK
			8,	//	p.3	BCM.2 PURPLE I2C SDA (fixed pull-up)
			9,	// 	p.5	BCM.3 BLUE I2C SCL (fixed pull-up)
			7,	// 	p.7 BCM.4 GREEN GPCLK0
			0,	//	p.11 BCM.17 YELLOW
			2, 	// 	p.13 BCM.27 ORANGE
			3,	//	p.15 BCM.22 RED
			12	// 	p.19 BCM.10 BROWN MOSI
			};

	bool isRunning;
	bool stopRequested;

	unsigned int pressedRow;
	unsigned int pressedCol;
	bool somethingPressed;

public:

	static KeypadScanner * getInstance() {
		if (!instance) {
			syslog(LOG_INFO, "Intantiating KeypadScanner");
			instance = new KeypadScanner();
		}
		return instance;
	}

	void launchThread();
	void mainLoop();

	virtual ~KeypadScanner();

};

#endif /* PIXPLANEFMCCDU_SRC_KEYPADSCANNER_H_ */
