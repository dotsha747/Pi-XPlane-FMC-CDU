/*
 * LEDs.cpp
 *
 *  Created on: May 16, 2017
 *      Author: shahada
 */

#include <unistd.h>
#include <wiringPi.h>


#include "LEDs.h"

// initialize statics
LEDs * LEDs::instance = NULL;


LEDs::LEDs () {

	pinMode (LED_EXEC, OUTPUT);
	pinMode (LED_DSPY, OUTPUT);
	pinMode (LED_FAIL, OUTPUT);
	pinMode (LED_MSG, OUTPUT);
	pinMode (LED_OFST, OUTPUT);

	digitalWrite (LED_EXEC, 1);
	digitalWrite (LED_DSPY, 1);
	digitalWrite (LED_FAIL, 1);
	digitalWrite (LED_MSG, 1);
	digitalWrite (LED_OFST, 1);

	sleep (1);

	digitalWrite (LED_EXEC, 0);
	digitalWrite (LED_DSPY, 0);
	digitalWrite (LED_FAIL, 0);
	digitalWrite (LED_MSG, 0);
	digitalWrite (LED_OFST, 0);

}


LEDs::~LEDs () {

	digitalWrite (LED_EXEC, 0);
	digitalWrite (LED_DSPY, 0);
	digitalWrite (LED_FAIL, 0);
	digitalWrite (LED_MSG, 0);
	digitalWrite (LED_OFST, 0);


	pinMode (LED_EXEC, INPUT);
	pinMode (LED_DSPY, INPUT);
	pinMode (LED_FAIL, INPUT);
	pinMode (LED_MSG, INPUT);
	pinMode (LED_OFST, INPUT);
}

void LEDs::setLED (LEDs::LedTypes led, bool on) {

	digitalWrite (led, on);
}
