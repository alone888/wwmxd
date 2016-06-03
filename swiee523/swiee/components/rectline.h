#ifndef RECTLINE_H
#define RECTLINE_H

#include "component.h"


class RectLine : public Component  {
public:
  RectLine();
 ~RectLine();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
