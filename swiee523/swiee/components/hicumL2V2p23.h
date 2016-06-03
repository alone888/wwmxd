#ifndef hicumL2V2p23_H
#define hicumL2V2p23_H

#include "component.h"

class hicumL2V2p23 : public Component
{
  public:
    hicumL2V2p23();
    ~hicumL2V2p23() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
};

#endif /* hicumL2V2p23_H */
