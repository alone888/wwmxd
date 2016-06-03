#ifndef MSSTEP_H
#define MSSTEP_H

#include "component.h"


class MSstep : public Component  {
public:
  MSstep();
  ~MSstep();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
