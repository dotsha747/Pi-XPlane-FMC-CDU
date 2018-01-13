/*
 * SplashFMC.h
 *
 *  Created on: Jan 8, 2018
 *      Author: shahada
 */

#ifndef PIXPLANEFMCCDU_SPLASHFMC_H_
#define PIXPLANEFMCCDU_SPLASHFMC_H_

#include <string>
#include "AbstractFMC.h"

class SplashFMC : public AbstractFMC {
protected:
	int numCols;
public:
	SplashFMC();
	virtual ~SplashFMC();

	virtual std::string getName() {
		return "SplashFMC";
	};

	virtual void init();
	virtual void deInit ();
	virtual void receiveData (time_t time, std::string type, std::string dataref, std::string value);
	virtual void keyPressEvent (int row, int col);
	virtual void keyReleaseEvent (int row, int col);

	virtual void drawCenteredText(int line, std::string text);

};

#endif /* PIXPLANEFMCCDU_SPLASHFMC_H_ */
