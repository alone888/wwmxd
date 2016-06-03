#ifndef hpribin4bit_H
#define hpribin4bit_H

#include "component.h"

class hpribin4bit : public Component
{
  public:
    hpribin4bit();
    ~hpribin4bit() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
    QString vhdlCode(int);
    QString verilogCode(int);
};

#endif /* hpribin4bit_H */
