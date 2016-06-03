#ifndef RFEDD2P_H
#define RFEDD2P_H

#include "component.h"


class RFedd2P : public MultiViewComponent  {
public:
  RFedd2P();
 ~RFedd2P() {};
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);

protected:
  QString netlist();
  void createSymbol();
};

#endif
