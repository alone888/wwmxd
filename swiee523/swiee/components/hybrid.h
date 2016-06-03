#ifndef HYBRID_H
#define HYBRID_H

#include "component.h"

class Hybrid : public Component  {
public:
  Hybrid();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
