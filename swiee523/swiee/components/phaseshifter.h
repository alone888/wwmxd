#ifndef PHASESHIFTER_H
#define PHASESHIFTER_H

#include "component.h"


class Phaseshifter : public Component  {
public:
  Phaseshifter();
  ~Phaseshifter();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
