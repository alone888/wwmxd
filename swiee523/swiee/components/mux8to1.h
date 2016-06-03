#ifndef mux8to1_H
#define mux8to1_H

#include "component.h"

class mux8to1 : public Component
{
  public:
    mux8to1();
    ~mux8to1() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
    QString vhdlCode(int);
    QString verilogCode(int);
};

#endif /* mux8to1_H */
