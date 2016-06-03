#ifndef LOGICAL_NAND_H
#define LOGICAL_NAND_H

#include "component.h"


class Logical_NAND : public GateComponent  {
public:
  Logical_NAND();
 ~Logical_NAND();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
