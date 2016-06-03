#ifndef D_FLIPFLOP_H
#define D_FLIPFLOP_H

#include "component.h"


class D_FlipFlop : public Component  {
public:
  D_FlipFlop();
 ~D_FlipFlop() {};
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);

protected:
  QString vhdlCode(int);
  QString verilogCode(int);
};

#endif
