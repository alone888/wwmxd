#ifndef logic_0_H
#define logic_0_H

#include "component.h"

class logic_0 : public Component
{
  public:
    logic_0();
    ~logic_0() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
    QString vhdlCode(int);
    QString verilogCode(int);
};

#endif /* logic_0_H */
