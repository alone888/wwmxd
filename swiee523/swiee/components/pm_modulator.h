#ifndef PM_MODULATOR_H
#define PM_MODULATOR_H

#include "component.h"


class PM_Modulator : public Component  {
public:
  PM_Modulator();
 ~PM_Modulator();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
