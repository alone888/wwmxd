#ifndef SUBCIRPORT_H
#define SUBCIRPORT_H

#include "component.h"


class SubCirPort : public MultiViewComponent  {
public:
  SubCirPort();
 ~SubCirPort() {};
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);

protected:
  QString netlist();
  QString vhdlCode(int);
  QString verilogCode(int);
  void createSymbol();
};

#endif
