#ifndef BJTSUB_H
#define BJTSUB_H

#include "component.h"


class Basic_BJT : public MultiViewComponent  {
public:
  Basic_BJT();
 ~Basic_BJT() {};
};

class BJTsub : public Basic_BJT  {
public:
  BJTsub();
 ~BJTsub() {};
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
  static Element* info_pnp(QString&, char* &, bool getNewOne=false);

protected:
  void createSymbol();
};

#endif
