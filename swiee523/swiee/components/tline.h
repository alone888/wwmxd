#ifndef TLINE_H
#define TLINE_H

#include "component.h"


class TLine : public Component  {
public:
  TLine();
  ~TLine();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
