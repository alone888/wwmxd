#ifndef RFEDD_H
#define RFEDD_H

#include "component.h"


class RFedd : public MultiViewComponent  {
public:
  RFedd();
 ~RFedd() {};
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);

protected:
  QString netlist();
  void createSymbol();
};

#endif
