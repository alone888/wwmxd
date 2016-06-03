#ifndef ETR_SIM_H
#define ETR_SIM_H

#include "component.h"


class ETR_Sim : public Component  {
public:
  ETR_Sim();
  ~ETR_Sim();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
//  void recreate(Schematic*);
};

#endif
