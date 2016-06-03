#ifndef COUPLER_H
#define COUPLER_H

#include "component.h"


class Coupler : public Component  {
public:
  Coupler();
 ~Coupler();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
