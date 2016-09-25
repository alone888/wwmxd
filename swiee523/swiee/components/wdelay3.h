#ifndef DELAY3_H
#define DELAY3_H

#include "component.h"


class wDelay3 : public Component  {
public:
	wDelay3();
	~wDelay3();
	Component* newOne();
	static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
