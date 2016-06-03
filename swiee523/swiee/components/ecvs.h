#ifndef ECVS_H
#define ECVS_H

#include "component.h"


class ecvs : public Component  {
public:
  ecvs();
  ~ecvs();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);

protected:
  QString netlist();
};

#endif

