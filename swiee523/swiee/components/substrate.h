#ifndef SUBSTRATE_H
#define SUBSTRATE_H

#include "component.h"


class Substrate : public Component  {
public:
  Substrate();
  ~Substrate();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
