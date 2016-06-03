#ifndef MUTUAL2_H
#define MUTUAL2_H

#include "component.h"


class Mutual2 : public Component  {
public:
  Mutual2();
 ~Mutual2();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
