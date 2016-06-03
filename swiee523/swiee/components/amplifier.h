#ifndef AMPLIFIER_H
#define AMPLIFIER_H

#include "component.h"


class Amplifier : public Component  {
public:
  Amplifier();
  ~Amplifier();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
