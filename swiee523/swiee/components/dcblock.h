#ifndef DCBLOCK_H
#define DCBLOCK_H

#include "component.h"


class dcBlock : public Component  {
public:
  dcBlock();
  ~dcBlock();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
