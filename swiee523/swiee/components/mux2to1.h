#ifndef mux2to1_H
#define mux2to1_H

#include "component.h"

class mux2to1 : public Component
{
  public:
    mux2to1();
    ~mux2to1() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
    QString vhdlCode(int);
    QString verilogCode(int);
};

#endif /* mux2to1_H */
