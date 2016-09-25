#ifndef DELAY4_H
#define DELAY4_H

#include "component.h"


class wDelay4 : public Component  {
public:
	wDelay4();
	~wDelay4();
	Component* newOne();
	static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
