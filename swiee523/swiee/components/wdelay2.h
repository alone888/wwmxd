#ifndef DELAY2_H
#define DELAY2_H

#include "component.h"


class wDelay2 : public Component  {
public:
	wDelay2();
	~wDelay2();
	Component* newOne();
	static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
