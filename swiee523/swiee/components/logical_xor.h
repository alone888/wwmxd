#ifndef LOGICAL_XOR_H
#define LOGICAL_XOR_H

#include "component.h"


class Logical_XOR : public GateComponent  {
public:
  Logical_XOR();
 ~Logical_XOR();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
