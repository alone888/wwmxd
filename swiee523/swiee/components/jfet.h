#ifndef JFET_H
#define JFET_H

#include "component.h"


class JFET : public MultiViewComponent {
public:
  JFET();
 ~JFET() {};
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
  static Element* info_p(QString&, char* &, bool getNewOne=false);

protected:
  void createSymbol();
};

#endif
