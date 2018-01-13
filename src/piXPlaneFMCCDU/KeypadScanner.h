/*
 * KeypadScanner.h
 *
 *  Created on: May 5, 2017
 *      Author: shahada
 */

#ifndef PIXPLANEFMCCDU_SRC_KEYPADSCANNER_H_
#define PIXPLANEFMCCDU_SRC_KEYPADSCANNER_H_

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
