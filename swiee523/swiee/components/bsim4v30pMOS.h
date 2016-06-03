#ifndef bsim4v30pMOS_H
#define bsim4v30pMOS_H

#include "component.h"

class bsim4v30pMOS : public Component
{
  public:
    bsim4v30pMOS();
    ~bsim4v30pMOS() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
};

#endif /* bsim4v30pMOS_H */
