#ifndef MSRSTUB_H
#define MSRSTUB_H

#include "component.h"

class MSrstub : public Component  {
public:
  MSrstub();
  ~MSrstub();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
