#ifndef LOGICAL_INV_H
#define LOGICAL_INV_H

#include "component.h"


class Logical_Inv : public MultiViewComponent  {
public:
  Logical_Inv();
 ~Logical_Inv() {};
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);

protected:
  QString vhdlCode(int);
  QString verilogCode(int);
  void createSymbol();
};

#endif
