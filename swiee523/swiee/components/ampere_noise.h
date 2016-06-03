#ifndef AMPERE_NOISE_H
#define AMPERE_NOISE_H

#include "component.h"


class Ampere_noise : public Component  {
public:
  Ampere_noise();
  ~Ampere_noise();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
