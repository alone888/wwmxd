#ifndef GYRATOR_H
#define GYRATOR_H

#include "component.h"


class Gyrator : public Component  {
public:
  Gyrator();
  ~Gyrator();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
