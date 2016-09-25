#ifndef DELAY5_H
#define DELAY5_H

#include "component.h"


class wDelay5 : public Component  {
public:
	wDelay5();
	~wDelay5();
	Component* newOne();
	static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
