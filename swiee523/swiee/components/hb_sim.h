#ifndef HB_SIM_H
#define HB_SIM_H

#include "component.h"


class HB_Sim : public Component  {
public:
  HB_Sim();
  ~HB_Sim();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
