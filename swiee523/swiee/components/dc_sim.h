#ifndef DC_SIM_H
#define DC_SIM_H

#include "component.h"


class DC_Sim : public Component  {
public:
  DC_Sim();
  ~DC_Sim();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
