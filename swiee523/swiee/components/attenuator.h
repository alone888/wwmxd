#ifndef ATTENUATOR_H
#define ATTENUATOR_H

#include "component.h"


class Attenuator : public Component  {
public:
  Attenuator();
  ~Attenuator();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
