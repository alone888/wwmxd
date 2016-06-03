#ifndef LOGICAL_OR_H
#define LOGICAL_OR_H

#include "component.h"


class Logical_OR : public GateComponent  {
public:
  Logical_OR();
 ~Logical_OR();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
