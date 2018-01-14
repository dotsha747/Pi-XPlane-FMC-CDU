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

#include <stdio.h>
#include <string.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <vector>

using namespace std;

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

void readAll() {

	printf("-----Output (Columns)-----    ------Input (Rows)-----\n");

	// set columns as outputs, HIGH by default
	for (size_t pin = 0; pin < columnPins.size(); pin++) {
		digitalWrite(columnPins[pin], 1);
		pinMode(columnPins[pin], OUTPUT);
		printf("%02d ", columnPins[pin]);
	}

	printf("   ");

	// set rows as inputs, with Pull-UP enabled.
	for (size_t pin = 0; pin < rowPins.size(); pin++) {
		pinMode(rowPins[pin], INPUT);
		pullUpDnControl(rowPins[pin], PUD_UP);
		printf("%02d ", rowPins[pin]);
	}

	printf("\n");

	for (int activeCol = 0; activeCol < 9; activeCol++) {

		for (size_t pin = 0; pin < columnPins.size(); pin++) {
			digitalWrite(columnPins[pin], pin == activeCol ? 0 : 1);
			delayMicroseconds(15);
			printf("%2d ", digitalRead(columnPins[pin]));
		}

		printf("   ");

		// set rows as inputs, with Pull-UP enabled.
		for (size_t pin = 0; pin < rowPins.size(); pin++) {
			printf("%2d ", digitalRead(rowPins[pin]));
		}

		printf("\n");

	}

	exit(1);

}

void scanning() {

// set columns as outputs, HIGH by default
	for (size_t pin = 0; pin < columnPins.size(); pin++) {
		digitalWrite(columnPins[pin], 1);
		pinMode(columnPins[pin], OUTPUT);
	}

// set rows as inputs, with Pull-UP enabled.
	for (size_t pin = 0; pin < rowPins.size(); pin++) {
		pinMode(rowPins[pin], INPUT);
		pullUpDnControl(rowPins[pin], PUD_UP);
	}

	unsigned int pressedRow;
	unsigned int pressedCol;
	bool somethingPressed = false;

	printf(
			"Scanning keyboard ... press any key on the FMC keypad to test.\nPress Control-C (on the regular keyboard) to abort.\n");

	while (true) {

		// iterate through the columns
		bool gotPress = false;
		for (size_t nowCol = 0; nowCol < columnPins.size(); nowCol++) {

			// set the current column to LOW
			digitalWrite(columnPins[nowCol], 0);

			// go through the rows, see who is low (pressed)

			for (size_t nowRow = 0; nowRow < rowPins.size(); nowRow++) {

				// delay a bit for the GPIO state to settle
				delayMicroseconds(5);
				int status = digitalRead(rowPins[nowRow]);

				// something pressed
				if (status == 0) {
					gotPress = true;
					if (somethingPressed == true) {
						if (pressedRow == nowRow && pressedCol == nowCol) {

							// previous key still being held down.

						} else {
							// multiple keys being pressed or another key pressed
							// without the earlier being released. Do nothing.

							printf("Multiple keys pressed.\n");
						}
					} else {
						// looks like a new key has been pressed
						pressedRow = nowRow;
						pressedCol = nowCol;
						somethingPressed = true;

						printf("KEY PRESS   row=%02d col=%02d\n", nowRow + 1,
								nowCol + 1);

						// quick and dirty delay for 5ms to debounce
						delayMicroseconds(5000);
					}

				}

			}
			// restore the current column to HIGH
			digitalWrite(columnPins[nowCol], 1);

		}

		if (!gotPress) {
			// nothing pressed this scan. If something was pressed before, consider
			// it released.
			if (somethingPressed) {
				somethingPressed = false;

				printf("KEY RELEASE row=%02d col=%02d\n", pressedRow + 1,
						pressedCol + 1);
			}
		}

	}

}

int main(int argc, char * argv[]) {

	if (argc != 2
			|| (strcmp(argv[1], "readall") != 0 && strcmp(argv[1], "scan") != 0)) {
		fprintf(stderr, "\nUsage: testFMCKeypad (readall|scan)\n\n");
		fprintf(stderr,
				"testFMCKeypad Copyright (C) 2017,2018 shahada abubakar\n\n");
		fprintf(stderr, "This program comes with ABSOLUTELY NO WARRANTY.\n\n");
		fprintf(stderr,
				"This is free software, and you are welcome to redistribute it under\n");
		fprintf(stderr,
				"certain conditions. See /usr/share/doc/pi-xplane-fmc-cdu/copyright or\n");
		fprintf(stderr,
				"https://github.com/dotsha747/Pi-XPlane-FMC-CDU for more details.\n");
		exit(1);
	}

	wiringPiSetup();

	if (strcmp(argv[1], "readall") == 0) {
		readAll();
	} else {
		scanning();
	}

}
