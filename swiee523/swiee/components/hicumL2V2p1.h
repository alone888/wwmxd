#ifndef hicumL2V2p1_H
#define hicumL2V2p1_H

#include "component.h"

class hicumL2V2p1 : public Component
{
  public:
    hicumL2V2p1();
   ~hicumL2V2p1() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
};

#endif /* hicumL2V2p1_H */
