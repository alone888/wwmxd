#ifndef VEXP_H
#define VEXP_H

#include "component.h"


class vExp : public Component  {
public:
  vExp();
  ~vExp();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
