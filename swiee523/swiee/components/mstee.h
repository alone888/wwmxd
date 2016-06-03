#ifndef MSTEE_H
#define MSTEE_H

#include "component.h"


class MStee : public MultiViewComponent  {
public:
  MStee();
 ~MStee();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);

protected:
  void createSymbol();
};

#endif
