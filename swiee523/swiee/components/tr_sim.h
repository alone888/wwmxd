#ifndef TR_SIM_H
#define TR_SIM_H

#include "component.h"


class TR_Sim : public Component  {
public:
  TR_Sim();
  ~TR_Sim();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
  void recreate(Schematic*);
};

#endif
