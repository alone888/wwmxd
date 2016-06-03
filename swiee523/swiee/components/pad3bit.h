#ifndef pad3bit_H
#define pad3bit_H

#include "component.h"

class pad3bit : public Component
{
  public:
    pad3bit();
    ~pad3bit() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
    QString vhdlCode(int);
    QString verilogCode(int);
};

#endif /* pad3bit_H */
