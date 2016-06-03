#ifndef HBT_X_H
#define HBT_X_H

#include "component.h"

class HBT_X : public Component
{
  public:
    HBT_X();
   ~HBT_X() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
};

#endif /* HBT_X_H */
