#ifndef ISOLATOR_H
#define ISOLATOR_H

#include "component.h"


class Isolator : public Component  {
public:
  Isolator();
  ~Isolator();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
