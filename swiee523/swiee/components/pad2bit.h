#ifndef pad2bit_H
#define pad2bit_H

#include "component.h"

class pad2bit : public Component
{
  public:
    pad2bit();
    ~pad2bit() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
    QString vhdlCode(int);
    QString verilogCode(int);
};

#endif /* pad2bit_H */
