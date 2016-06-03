#ifndef DIGI_SIM_H
#define DIGI_SIM_H

#include "component.h"


class Digi_Sim : public Component  {
public:
  Digi_Sim();
 ~Digi_Sim();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
