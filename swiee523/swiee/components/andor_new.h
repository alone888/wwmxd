#ifndef andor_new_H
#define andor_new_H

#include "component.h"

class andor_new : public Component
{
  public:
    andor_new();
    ~andor_new() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
    QString vhdlCode(int);
    QString verilogCode(int);
};

#endif /* andor_new_H */
