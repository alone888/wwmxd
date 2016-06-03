#ifndef SUBCIRCUIT_H
#define SUBCIRCUIT_H

#include "component.h"


class Subcircuit : public MultiViewComponent  {
public:
  Subcircuit();
 ~Subcircuit() {};
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);

  QString getSubcircuitFile();

protected:
  QString netlist();
  QString vhdlCode(int);
  QString verilogCode(int);
  void createSymbol();
  void remakeSymbol(int No);
  int  loadSymbol(const QString&);
};

#endif
