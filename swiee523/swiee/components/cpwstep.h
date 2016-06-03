#ifndef CPWSTEP_H
#define CPWSTEP_H

#include "component.h"


class CPWstep : public Component  {
public:
  CPWstep();
  ~CPWstep();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
