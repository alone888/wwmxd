#ifndef comp_4bit_H
#define comp_4bit_H

#include "component.h"

class comp_4bit : public Component
{
  public:
    comp_4bit();
    ~comp_4bit() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
    QString vhdlCode(int);
    QString verilogCode(int);
};

#endif /* comp_4bit_H */
