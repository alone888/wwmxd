#ifndef logic_1_H
#define logic_1_H

#include "component.h"

class logic_1 : public Component
{
  public:
    logic_1();
    ~logic_1() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
    QString vhdlCode(int);
    QString verilogCode(int);
};

#endif /* logic_1_H */
