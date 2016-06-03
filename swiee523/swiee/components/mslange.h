#ifndef MSLANGE_H
#define MSLANGE_H

#include "component.h"


class MSlange : public Component  {
public:
  MSlange();
  ~MSlange();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
