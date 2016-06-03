#ifndef MSCROSS_H
#define MSCROSS_H

#include "component.h"


class MScross : public MultiViewComponent  {
public:
  MScross();
 ~MScross();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);

protected:
  void createSymbol();
};

#endif
