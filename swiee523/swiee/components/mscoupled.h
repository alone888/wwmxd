#ifndef MSCOUPLED_H
#define MSCOUPLED_H

#include "component.h"


class MScoupled : public Component  {
public:
  MScoupled();
  ~MScoupled();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
