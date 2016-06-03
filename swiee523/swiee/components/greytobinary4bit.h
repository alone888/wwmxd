#ifndef greytobinary4bit_H
#define greytobinary4bit_H

#include "component.h"

class greytobinary4bit : public Component
{
  public:
    greytobinary4bit();
    ~greytobinary4bit() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
    QString vhdlCode(int);
    QString verilogCode(int);
};

#endif /* greytobinary4bit_H */
