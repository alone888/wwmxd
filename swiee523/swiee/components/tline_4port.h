#ifndef TLINE_4PORT_H
#define TLINE_4PORT_H

#include "component.h"


class TLine_4Port : public Component  {
public:
  TLine_4Port();
 ~TLine_4Port();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
