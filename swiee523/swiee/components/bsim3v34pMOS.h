#ifndef bsim3v34pMOS_H
#define bsim3v34pMOS_H

#include "component.h"

class bsim3v34pMOS : public Component
{
  public:
    bsim3v34pMOS();
    ~bsim3v34pMOS() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
};

#endif /* bsim3v34pMOS_H */
