#ifndef MUTUAL_H
#define MUTUAL_H

#include "component.h"


class Mutual : public Component  {
public:
  Mutual();
 ~Mutual();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
