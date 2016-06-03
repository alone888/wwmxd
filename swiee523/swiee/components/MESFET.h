#ifndef MESFET_H
#define MESFET_H

#include "component.h"

class MESFET : public Component
{
  public:
    MESFET();
    ~MESFET() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
};

#endif /* MESFET_H */
