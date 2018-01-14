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

#include <unistd.h>
#include <wiringPi.h>

#include "LEDs.h"

// initialize statics
LEDs * LEDs::instance = NULL;

LEDs::LEDs() {

	pinMode(LED_EXEC, OUTPUT);
	pinMode(LED_DSPY, OUTPUT);
	pinMode(LED_FAIL, OUTPUT);
	pinMode(LED_MSG, OUTPUT);
	pinMode(LED_OFST, OUTPUT);

	digitalWrite(LED_EXEC, 1);
	digitalWrite(LED_DSPY, 1);
	digitalWrite(LED_FAIL, 1);
	digitalWrite(LED_MSG, 1);
	digitalWrite(LED_OFST, 1);

	sleep(1);

	digitalWrite(LED_EXEC, 0);
	digitalWrite(LED_DSPY, 0);
	digitalWrite(LED_FAIL, 0);
	digitalWrite(LED_MSG, 0);
	digitalWrite(LED_OFST, 0);

}

LEDs::~LEDs() {

	digitalWrite(LED_EXEC, 0);
	digitalWrite(LED_DSPY, 0);
	digitalWrite(LED_FAIL, 0);
	digitalWrite(LED_MSG, 0);
	digitalWrite(LED_OFST, 0);

	pinMode(LED_EXEC, INPUT);
	pinMode(LED_DSPY, INPUT);
	pinMode(LED_FAIL, INPUT);
	pinMode(LED_MSG, INPUT);
	pinMode(LED_OFST, INPUT);
}

void LEDs::setLED(LEDs::LedTypes led, bool on) {

	digitalWrite(led, on);
}

void LEDs::setAllLEDs(bool on) {

	setLED(LED_EXEC, on);
	setLED(LED_DSPY, on);
	setLED(LED_FAIL, on);
	setLED(LED_MSG, on);
	setLED(LED_OFST, on);
}
