#ifndef ha1b_H
#define ha1b_H

#include "component.h"

class ha1b : public Component
{
  public:
    ha1b();
    ~ha1b() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
    QString vhdlCode(int);
    QString verilogCode(int);
};

#endif /* ha1b_H */
