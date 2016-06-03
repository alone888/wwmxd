#ifndef bsim3v34nMOS_H
#define bsim3v34nMOS_H

#include "component.h"

class bsim3v34nMOS : public Component
{
  public:
    bsim3v34nMOS();
    ~bsim3v34nMOS() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
};

#endif /* bsim3v34nMOS_H */
