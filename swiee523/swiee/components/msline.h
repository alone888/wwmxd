#ifndef MSLINE_H
#define MSLINE_H

#include "component.h"


class MSline : public Component  {
public:
  MSline();
  ~MSline();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
