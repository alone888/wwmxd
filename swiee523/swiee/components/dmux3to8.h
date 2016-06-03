#ifndef dmux3to8_H
#define dmux3to8_H
#include "component.h"

class dmux3to8 : public Component
{
  public:
    dmux3to8();
    ~dmux3to8() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
    QString vhdlCode(int);
    QString verilogCode(int);
};

#endif /* dmux3to8_H */
