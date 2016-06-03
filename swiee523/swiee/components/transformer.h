#ifndef TRANSFORMER_H
#define TRANSFORMER_H

#include "component.h"


class Transformer : public Component  {
public:
  Transformer();
  ~Transformer();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
