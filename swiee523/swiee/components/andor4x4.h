#ifndef andor4x4_H
#define andor4x4_H

#include "component.h"

class andor4x4 : public Component
{
  public:
    andor4x4();
    ~andor4x4() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
    QString vhdlCode(int);
    QString verilogCode(int);
};

#endif /* andor4x4_H */
