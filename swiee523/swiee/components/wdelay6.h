#ifndef DELAY6_H
#define DELAY6_H

#include "component.h"


class wDelay6 : public Component  {
public:
	wDelay6();
	~wDelay6();
	Component* newOne();
	static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
