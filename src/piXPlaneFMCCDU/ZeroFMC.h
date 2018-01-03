/*
 * ZeroFMC.h
 *
 *  Created on: May 31, 2017
 *      Author: shahada
 */

#ifndef PIXPLANEFMCCDU_SRC_ZEROFMC_H_
#define PIXPLANEFMCCDU_SRC_ZEROFMC_H_

#include "AbstractFMC.h"

class ZeroFMC: public AbstractFMC {
public:
	ZeroFMC();
	virtual ~ZeroFMC();

	virtual void init ();
	virtual std::string getName();

	virtual void receiveData (time_t time, std::string type, std::string dataref, std::string value);

	virtual void keyPressEvent (int row, int col);
	virtual void keyReleaseEvent (int row, int col);



};

#endif /* PIXPLANEFMCCDU_SRC_ZEROFMC_H_ */
