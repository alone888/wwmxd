#ifndef AC_SIM_H
#define AC_SIM_H

#include "component.h"


class AC_Sim : public Component  {
public:
  AC_Sim();
 ~AC_Sim();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
  void recreate(Schematic*);
};

#endif
