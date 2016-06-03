#ifndef OPAMP_H
#define OPAMP_H

#include "component.h"


class OpAmp : public Component  {
public:
  OpAmp();
  ~OpAmp();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
