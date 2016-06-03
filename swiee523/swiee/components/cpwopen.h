#ifndef CPWOPEN_H
#define CPWOPEN_H

#include "component.h"


class CPWopen : public Component  {
public:
  CPWopen();
  ~CPWopen();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
