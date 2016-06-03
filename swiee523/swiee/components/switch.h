#ifndef SWITCH_H
#define SWITCH_H

#include "component.h"


class Switch : public MultiViewComponent  {
public:
  Switch();
 ~Switch() {};
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);

protected:
  QString netlist();
  void createSymbol();
};

#endif
