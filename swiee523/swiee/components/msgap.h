#ifndef MSGAP_H
#define MSGAP_H

#include "component.h"


class MSgap : public Component  {
public:
  MSgap();
  ~MSgap();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
