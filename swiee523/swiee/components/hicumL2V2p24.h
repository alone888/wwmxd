#ifndef hicumL2V2p24_H
#define hicumL2V2p24_H

#include "component.h"

class hicumL2V2p24 : public Component
{
  public:
    hicumL2V2p24();
    ~hicumL2V2p24() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
};

#endif /* hicumL2V2p24_H */
