#ifndef MSOPEN_H
#define MSOPEN_H

#include "component.h"


class MSopen : public Component  {
public:
  MSopen();
  ~MSopen();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
