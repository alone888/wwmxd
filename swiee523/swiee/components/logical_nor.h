#ifndef LOGICAL_NOR_H
#define LOGICAL_NOR_H

#include "component.h"


class Logical_NOR : public GateComponent  {
public:
  Logical_NOR();
 ~Logical_NOR();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
