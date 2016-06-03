#ifndef IEXP_H
#define IEXP_H

#include "component.h"


class iExp : public Component  {
public:
  iExp();
  ~iExp();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
