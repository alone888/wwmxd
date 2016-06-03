#ifndef COPLANAR_H
#define COPLANAR_H

#include "component.h"


class Coplanar : public Component  {
public:
  Coplanar();
  ~Coplanar();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
