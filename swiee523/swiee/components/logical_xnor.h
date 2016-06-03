#ifndef LOGICAL_XNOR_H
#define LOGICAL_XNOR_H

#include "component.h"


class Logical_XNOR : public GateComponent  {
public:
  Logical_XNOR();
 ~Logical_XNOR();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
