#ifndef DELAY_H
#define DELAY_H

#include "component.h"


class wDelay : public Component  {
public:
	wDelay();
	~wDelay();
	Component* newOne();
	static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
