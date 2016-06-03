#ifndef dmux2to4_H
#define dmux2to4_H

#include "component.h"

class dmux2to4 : public Component
{
  public:
    dmux2to4();
    ~dmux2to4() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
    QString vhdlCode(int);
    QString verilogCode(int);
};

#endif /* dmux2to4_H */
