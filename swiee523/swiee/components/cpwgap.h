#ifndef CPWGAP_H
#define CPWGAP_H

#include "component.h"


class CPWgap : public Component  {
public:
  CPWgap();
  ~CPWgap();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
