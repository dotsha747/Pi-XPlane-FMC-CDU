/*

 testFMCLED.cpp

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

#include <stdio.h>
#include <string.h>
#include <wiringPi.h>
#include <stdlib.h>

using namespace std;

enum LedTypes
	: int {
	LED_EXEC = 13,	// p.21 bcm.9
	LED_MSG = 14, 	// p.3 bcm.11
	LED_DSPY = 21,	// p.29 bcm.5
	LED_OFST = 22, 	// p.31 bcm.6
	LED_FAIL = 23,	// p.33 bcm.13
	LED_BACK = 24   
};

int main(int argc, char * argv[]) {

	if (argc != 3) {
		fprintf (stderr, "\nUsage: testFMCLED (exec|dspy|fail|msg|ofst) (on|off)\n\n");
		fprintf (stderr, "testFMCLED Copyright (C) 2017,2018 shahada abubakar\n\n");
		fprintf (stderr, "This program comes with ABSOLUTELY NO WARRANTY.\n\n");
		fprintf (stderr, "This is free software, and you are welcome to redistribute it under\n");
		fprintf (stderr, "certain conditions. See /usr/share/doc/pi-xplane-fmc-cdu/copyright or\n");
		fprintf (stderr, "https://github.com/dotsha747/Pi-XPlane-FMC-CDU for more details.\n");
		exit(1);
	}

	wiringPiSetup();

	int state = strcmp (argv[2], "on") == 0;
	printf ("Switching %s %s\n", argv[1], state ? "on" : "off");

	if (strcmp(argv[1], "exec") == 0) {
		pinMode(LED_EXEC, OUTPUT);
		digitalWrite(LED_EXEC, state);
	} else if (strcmp(argv[1], "dspy") == 0) {
		pinMode(LED_DSPY, OUTPUT);
		digitalWrite(LED_DSPY, state);
	} else if (strcmp(argv[1], "fail") == 0) {
		pinMode(LED_FAIL, OUTPUT);
		digitalWrite(LED_FAIL, state);
	} else if (strcmp(argv[1], "ofst") == 0) {
		pinMode(LED_OFST, OUTPUT);
		digitalWrite(LED_OFST, state);
	} else if (strcmp(argv[1], "msg") == 0) {
		pinMode(LED_MSG, OUTPUT);
		digitalWrite(LED_MSG, state);
	} else if (strcmp(argv[1], "back") == 0) {
		pinMode(LED_BACK, OUTPUT);
		digitalWrite(LED_BACK, state);
	} else if (strcmp(argv[1], "all") == 0) {
		pinMode(LED_DSPY, OUTPUT);
		pinMode(LED_EXEC, OUTPUT);
		pinMode(LED_FAIL, OUTPUT);
		pinMode(LED_OFST, OUTPUT);
		pinMode(LED_MSG, OUTPUT);
		pinMode(LED_BACK, OUTPUT);
		digitalWrite(LED_EXEC, state);
		digitalWrite(LED_DSPY, state);
		digitalWrite(LED_FAIL, state);
		digitalWrite(LED_OFST, state);
		digitalWrite(LED_MSG, state);
		digitalWrite(LED_BACK, state);
	}
}
