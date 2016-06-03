#ifndef MOSFET_SUB_H
#define MOSFET_SUB_H

#include "component.h"


class Basic_MOSFET : public MultiViewComponent {
public:
  Basic_MOSFET();
 ~Basic_MOSFET() {};
};

class MOSFET_sub : public Basic_MOSFET {
public:
  MOSFET_sub();
 ~MOSFET_sub() {};
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
  static Element* info_p(QString&, char* &, bool getNewOne=false);
  static Element* info_depl(QString&, char* &, bool getNewOne=false);

protected:
  void createSymbol();
};

#endif
