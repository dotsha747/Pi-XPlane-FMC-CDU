/*
 * ZiboFMC.h
 *
 *  Created on: Jul 26, 2017
 *      Author: shahada
 */

#ifndef PIXPLANEFMCCDU_SRC_ZIBOFMC_H_
#define PIXPLANEFMCCDU_SRC_ZIBOFMC_H_

#include <map>

#include "AbstractFMC.h"

class ZiboFMC: public AbstractFMC {
protected:
	std::string largeLineCache[7];
	std::map<int, std::map<int, std::string>> keyInfo;
public:
	ZiboFMC();
	virtual ~ZiboFMC();

	virtual std::string getName();
	virtual void init();

	virtual void keyPressEvent(int, int);
	virtual void keyReleaseEvent(int, int);
	virtual void receiveData(time_t, std::string, std::string, std::string);

	virtual std::string mixLargeSmallLines (std::string large, std::string small);

	virtual void subscribe();
	virtual void unsubscribe();

};

#endif /* PIXPLANEFMCCDU_SRC_ZIBOFMC_H_ */
