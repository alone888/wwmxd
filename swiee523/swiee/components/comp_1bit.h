#ifndef comp_1bit_H
#define comp_1bit_H

#include "component.h"

class comp_1bit : public Component
{
  public:
    comp_1bit();
    ~comp_1bit() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
    QString vhdlCode(int);
    QString verilogCode(int);
};

#endif /* comp_1bit_H */
