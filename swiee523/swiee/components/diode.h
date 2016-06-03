#ifndef DIODE_H
#define DIODE_H

#include "component.h"


class Diode : public MultiViewComponent  {
public:
  Diode();
 ~Diode() {};
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);

protected:
  void createSymbol();
};

#endif
