#ifndef SP_SIM_H
#define SP_SIM_H

#include "component.h"


class SP_Sim : public Component  {
public:
  SP_Sim();
  ~SP_Sim();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
  void recreate(Schematic*);
};

#endif
