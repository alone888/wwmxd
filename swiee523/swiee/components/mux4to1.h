#ifndef mux4to1_H
#define mux4to1_H

#include "component.h"

class mux4to1 : public Component
{
  public:
    mux4to1();
    ~mux4to1() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
    QString vhdlCode(int);
    QString verilogCode(int);
};

#endif /* mux4to1_H */
