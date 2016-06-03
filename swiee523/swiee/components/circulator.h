#ifndef CIRCULATOR_H
#define CIRCULATOR_H

#include "component.h"


class Circulator : public Component  {
public:
  Circulator();
  ~Circulator();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
