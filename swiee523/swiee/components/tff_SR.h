#ifndef tff_SR_H
#define tff_SR_H

#include "component.h"

class tff_SR : public Component
{
  public:
    tff_SR();
    ~tff_SR() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
    QString vhdlCode(int);
    QString verilogCode(int);
};

#endif /* tff_SR_H */
