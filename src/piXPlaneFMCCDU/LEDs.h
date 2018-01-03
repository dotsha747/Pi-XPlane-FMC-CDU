/*
 * LEDs.h
 *
 *  Created on: May 16, 2017
 *      Author: shahada
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

};

#endif /* PIXPLANEFMCCDU_SRC_LEDS_H_ */
