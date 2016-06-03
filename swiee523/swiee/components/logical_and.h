#ifndef LOGICAL_AND_H
#define LOGICAL_AND_H

#include "component.h"


class Logical_AND : public GateComponent  {
public:
  Logical_AND();
 ~Logical_AND();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
