#ifndef BONDWIRE_H
#define BONDWIRE_H

#include "component.h"


class BondWire : public Component  {
public:
  BondWire();
 ~BondWire() {};
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
