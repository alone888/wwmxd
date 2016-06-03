#ifndef THYRISTOR_H
#define THYRISTOR_H

#include "component.h"


class Thyristor : public Component  {
public:
  Thyristor();
 ~Thyristor() {};
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
