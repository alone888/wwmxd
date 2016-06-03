#ifndef pad4bit_H
#define pad4bit_H

#include "component.h"

class pad4bit : public Component
{
  public:
    pad4bit();
    ~pad4bit() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
    QString vhdlCode(int);
    QString verilogCode(int);
};

#endif /* pad4bit_H */
