#ifndef SYMTRAFO_H
#define SYMTRAFO_H

#include "component.h"


class symTrafo : public Component  {
public:
  symTrafo();
  ~symTrafo();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
