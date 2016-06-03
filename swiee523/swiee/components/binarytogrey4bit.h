#ifndef binarytogrey4bit_H
#define binarytogrey4bit_H

#include "component.h"

class binarytogrey4bit : public Component
{
  public:
    binarytogrey4bit();
    ~binarytogrey4bit() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
    QString vhdlCode(int);
    QString verilogCode(int);
};

#endif /* binarytogrey4bit_H */
