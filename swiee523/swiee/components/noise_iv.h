#ifndef NOISE_IV_H
#define NOISE_IV_H

#include "component.h"


class Noise_iv : public Component  {
public:
  Noise_iv();
 ~Noise_iv();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
