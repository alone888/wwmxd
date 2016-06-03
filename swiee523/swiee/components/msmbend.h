#ifndef MSMBEND_H
#define MSMBEND_H

#include "component.h"


class MSmbend : public Component  {
public:
  MSmbend();
  ~MSmbend();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
