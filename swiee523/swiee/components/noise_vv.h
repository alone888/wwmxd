#ifndef NOISE_VV_H
#define NOISE_VV_H

#include "component.h"


class Noise_vv : public Component  {
public:
  Noise_vv();
 ~Noise_vv();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
