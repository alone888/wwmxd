#ifndef andor4x2_H
#define andor4x2_H

#include "component.h"

class andor4x2 : public Component
{
  public:
    andor4x2();
    ~andor4x2() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
    QString vhdlCode(int);
    QString verilogCode(int);
};

#endif /* andor4x2_H */
