#ifndef VPULSE_H
#define VPULSE_H

#include "component.h"


class vPulse : public Component  {
public:
  vPulse();
  ~vPulse();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
