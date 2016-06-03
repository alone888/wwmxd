#ifndef CPWSHORT_H
#define CPWSHORT_H

#include "component.h"


class CPWshort : public Component  {
public:
  CPWshort();
  ~CPWshort();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
