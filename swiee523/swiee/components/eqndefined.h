#ifndef EQNDEFINED_H
#define EQNDEFINED_H

#include "component.h"


class EqnDefined : public MultiViewComponent  {
public:
  EqnDefined();
 ~EqnDefined() {};
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);

protected:
  QString netlist();
  void createSymbol();
};

#endif
