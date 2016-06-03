#ifndef DCFEED_H
#define DCFEED_H

#include "component.h"


class dcFeed : public Component  {
public:
  dcFeed();
  ~dcFeed();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif
