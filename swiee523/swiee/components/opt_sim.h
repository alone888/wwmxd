#ifndef OPT_SIM_H
#define OPT_SIM_H

#include "component.h"


class Optimize_Sim : public Component  {
public:
  Optimize_Sim();
 ~Optimize_Sim();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
  bool createASCOFiles();
  bool createASCOnetlist();
  bool loadASCOout();

protected:
  QString netlist();
};

#endif
