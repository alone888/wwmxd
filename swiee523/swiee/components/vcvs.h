#ifndef VCVS_H
#define VCVS_H

#include "component.h"


class VCVS : public Component  {
public:
  VCVS();
  ~VCVS();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
