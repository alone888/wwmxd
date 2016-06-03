#ifndef DIGI_SOURCE_H
#define DIGI_SOURCE_H

#include "component.h"


class Digi_Source : public Component  {
public:
  Digi_Source();
 ~Digi_Source();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);

protected:
  QString netlist();
  QString vhdlCode(int);
  QString verilogCode(int);
};

#endif
