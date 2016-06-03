#ifndef comp_2bit_H
#define comp_2bit_H

#include "component.h"

class comp_2bit : public Component
{
  public:
    comp_2bit();
    ~comp_2bit() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
    QString vhdlCode(int);
    QString verilogCode(int);
};

#endif /* comp_2bit_H */
