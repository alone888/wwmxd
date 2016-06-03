#ifndef BJT_H
#define BJT_H

#include "bjtsub.h"


class BJT : public Basic_BJT  {
public:
  BJT();
 ~BJT() {};
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
  static Element* info_pnp(QString&, char* &, bool getNewOne=false);

protected:
  void createSymbol();
  QString netlist();
};

#endif
