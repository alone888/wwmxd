#ifndef IPULSE_H
#define IPULSE_H

#include "component.h"


class iPulse : public Component  {
public:
  iPulse();
  ~iPulse();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
