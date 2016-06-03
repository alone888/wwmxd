#ifndef dmux4to16_H
#define dmux4to16_H

#include "component.h"

class dmux4to16 : public Component
{
  public:
    dmux4to16();
    ~dmux4to16() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
    QString vhdlCode(int);
    QString verilogCode(int);
};

#endif /* dmux4to16_H */
