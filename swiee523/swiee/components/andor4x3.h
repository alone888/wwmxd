#ifndef andor4x3_H
#define andor4x3_H

#include "component.h"

class andor4x3 : public Component
{
  public:
    andor4x3();
    ~andor4x3() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
    QString vhdlCode(int);
    QString verilogCode(int);
};

#endif /* andor4x3_H */
