#ifndef VCCS_H
#define VCCS_H

#include "component.h"


class VCCS : public Component  {
public:
  VCCS();
  ~VCCS();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
