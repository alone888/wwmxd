#ifndef nigbt_H
#define nigbt_H

#include "component.h"

class nigbt : public Component
{
  public:
    nigbt();
    ~nigbt() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
};

#endif /* nigbt_H */
