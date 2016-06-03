#ifndef COAXIALLINE_H
#define COAXIALLINE_H

#include "component.h"


class CoaxialLine : public Component  {
public:
  CoaxialLine();
 ~CoaxialLine();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
