#ifndef BIAST_H
#define BIAST_H

#include "component.h"


class BiasT : public Component  {
public:
  BiasT();
  ~BiasT();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
