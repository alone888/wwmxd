#ifndef DELAY1_H
#define DELAY1_H

#include "component.h"


class wDelay1 : public Component  {
public:
	wDelay1();
	~wDelay1();
	Component* newOne();
	static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
