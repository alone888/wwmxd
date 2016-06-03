#ifndef gatedDlatch_H
#define gatedDlatch_H

#include "component.h"

class gatedDlatch : public Component
{
  public:
    gatedDlatch();
    ~gatedDlatch() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
    QString vhdlCode(int);
    QString verilogCode(int);
};

#endif /* gatedDlatch_H */
