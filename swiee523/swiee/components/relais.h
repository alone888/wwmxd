#ifndef RELAIS_H
#define RELAIS_H

#include "component.h"


class Relais : public Component  {
public:
  Relais();
 ~Relais();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
