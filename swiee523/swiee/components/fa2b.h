#ifndef fa2b_H
#define fa2b_H

#include "component.h"

class fa2b : public Component
{
  public:
    fa2b();
    ~fa2b() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
    QString vhdlCode(int);
    QString verilogCode(int);
};

#endif /* fa2b_H */
