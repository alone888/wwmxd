#ifndef DIAC_H
#define DIAC_H

#include "component.h"


class Diac : public Component  {
public:
  Diac();
 ~Diac() {};
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
