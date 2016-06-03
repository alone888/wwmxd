#ifndef MSCORNER_H
#define MSCORNER_H

#include "component.h"


class MScorner : public Component  {
public:
  MScorner();
  ~MScorner();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
