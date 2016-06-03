#ifndef RLCG_H
#define RLCG_H

#include "component.h"

class RLCG : public Component  {
public:
  RLCG();
  ~RLCG();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
