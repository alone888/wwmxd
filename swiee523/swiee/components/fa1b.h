#ifndef fa1b_H
#define fa1b_H

#include "component.h"

class fa1b : public Component
{
  public:
    fa1b();
    ~fa1b() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
    QString vhdlCode(int);
    QString verilogCode(int);
};

#endif /* fa1b_H */
