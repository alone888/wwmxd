#ifndef CCCS_H
#define CCCS_H

#include "component.h"


class CCCS : public Component  {
public:
  CCCS();
  ~CCCS();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
