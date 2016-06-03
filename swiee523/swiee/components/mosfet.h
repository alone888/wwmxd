#ifndef MOSFET_H
#define MOSFET_H

#include "mosfet_sub.h"


class MOSFET : public Basic_MOSFET {
public:
  MOSFET();
 ~MOSFET() {};
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
  static Element* info_p(QString&, char* &, bool getNewOne=false);
  static Element* info_depl(QString&, char* &, bool getNewOne=false);

protected:
  void createSymbol();
  QString netlist();
};

#endif
