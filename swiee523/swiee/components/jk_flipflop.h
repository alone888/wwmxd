#ifndef JK_FLIPFLOP_H
#define JK_FLIPFLOP_H

#include "component.h"


class JK_FlipFlop : public Component  {
public:
  JK_FlipFlop();
 ~JK_FlipFlop() {};
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);

protected:
  QString vhdlCode(int);
  QString verilogCode(int);
};

#endif
