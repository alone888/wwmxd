#ifndef TWISTEDPAIR_H
#define TWISTEDPAIR_H

#include "component.h"


class TwistedPair : public Component  {
public:
  TwistedPair();
 ~TwistedPair();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
