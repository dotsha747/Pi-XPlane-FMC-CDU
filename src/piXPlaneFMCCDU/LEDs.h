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

#ifndef PIXPLANEFMCCDU_SRC_LEDS_H_
#define PIXPLANEFMCCDU_SRC_LEDS_H_

#include <stdlib.h>

/** @brief Turn On or Off LEDs on the CDU Panel
 *
 */

class LEDs {

private:
	LEDs(); // private so it cannot be called from outside
	LEDs (LEDs const &) {}; // private so it cannot be called from outside
	LEDs & operator= (LEDs const &) {
		abort();
	}; // assignment operator is private

	// @brief singleton instance
	static LEDs * instance;

public:

	// @brief return a pointer to the singleton instance
	static LEDs * getInstance() {
		if (!instance) {
			instance = new LEDs();
		}
		return instance;
	}

	virtual ~LEDs();

	// @brief mapping of LEDs and the WiringPi GPIO they connect to
	enum LedTypes : int {
		LED_EXEC = 13,	// p.21 bcm.9
		LED_MSG = 14, 	// p.3 bcm.11
		LED_DSPY = 21,	// p.29 bcm.5
		LED_OFST = 22, 	// p.31 bcm.6
		LED_FAIL = 23	// p.33 bcm.13
	};


	// @brief method to turn an LED on or off.
	void setLED (LEDs::LedTypes led, bool on);

	void setAllLEDs (bool on);

};

#endif /* PIXPLANEFMCCDU_SRC_LEDS_H_ */
