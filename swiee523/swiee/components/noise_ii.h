#ifndef NOISE_II_H
#define NOISE_II_H

#include "component.h"


class Noise_ii : public Component  {
public:
  Noise_ii();
 ~Noise_ii();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
