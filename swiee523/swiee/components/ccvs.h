#ifndef CCVS_H
#define CCVS_H

#include "component.h"


class CCVS : public Component  {
public:
  CCVS();
  ~CCVS();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
