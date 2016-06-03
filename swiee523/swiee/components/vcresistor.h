#ifndef VCRESISTOR_H
#define VCRESISTOR_H

#include "component.h"


class vcresistor : public Component  {
public:
  vcresistor();
  ~vcresistor();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
