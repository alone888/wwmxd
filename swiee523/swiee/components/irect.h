#ifndef IRECT_H
#define IRECT_H

#include "component.h"


class iRect : public Component  {
public:
  iRect();
  ~iRect();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
