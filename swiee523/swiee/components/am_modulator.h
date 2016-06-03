#ifndef AM_MODULATOR_H
#define AM_MODULATOR_H

#include "component.h"


class AM_Modulator : public Component  {
public:
  AM_Modulator();
 ~AM_Modulator();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
