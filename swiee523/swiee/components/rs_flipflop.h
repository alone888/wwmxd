#ifndef RS_FLIPFLOP_H
#define RS_FLIPFLOP_H

#include "component.h"


class RS_FlipFlop : public Component  {
public:
  RS_FlipFlop();
 ~RS_FlipFlop() {};
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);

protected:
  QString vhdlCode(int);
  QString verilogCode(int);
};

#endif
