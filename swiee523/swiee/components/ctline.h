#ifndef CTLINE_H
#define CTLINE_H

#include "component.h"

class CoupledTLine : public Component  {
public:
  CoupledTLine();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
