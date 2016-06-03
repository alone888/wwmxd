#ifndef VOLT_NOISE_H
#define VOLT_NOISE_H

#include "component.h"


class Volt_noise : public Component  {
public:
  Volt_noise();
  ~Volt_noise();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
