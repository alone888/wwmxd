#ifndef LOGICAL_BUF_H
#define LOGICAL_BUF_H

#include "component.h"


class Logical_Buf : public MultiViewComponent  {
public:
  Logical_Buf();
 ~Logical_Buf() {};
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);

protected:
  QString vhdlCode(int);
  QString verilogCode(int);
  void createSymbol();
};

#endif
