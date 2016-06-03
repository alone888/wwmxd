#ifndef TRIAC_H
#define TRIAC_H

#include "component.h"


class Triac : public Component  {
public:
  Triac();
 ~Triac() {};
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
