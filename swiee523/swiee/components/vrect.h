#ifndef VRECT_H
#define VRECT_H

#include "component.h"


class vRect : public Component  {
public:
  vRect();
  ~vRect();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
