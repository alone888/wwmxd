#ifndef bsim4v30nMOS_H
#define bsim4v30nMOS_H

#include "component.h"

class bsim4v30nMOS : public Component
{
  public:
    bsim4v30nMOS();
    ~bsim4v30nMOS() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
};

#endif /* bsim4v30nMOS_H */
