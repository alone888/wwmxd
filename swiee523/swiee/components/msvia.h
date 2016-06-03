#ifndef MSVIA_H
#define MSVIA_H

#include "component.h"


class MSvia : public Component  {
public:
  MSvia();
  ~MSvia();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);

protected:
  QString netlist();
};

#endif
