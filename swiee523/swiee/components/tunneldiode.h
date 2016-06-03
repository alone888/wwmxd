#ifndef TUNNELDIODE_H
#define TUNNELDIODE_H

#include "component.h"

class TunnelDiode : public Component  {
public:
  TunnelDiode();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
