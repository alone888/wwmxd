#ifndef jkff_SR_H
#define jkff_SR_H

#include "component.h"

class jkff_SR : public Component
{
  public:
    jkff_SR();
    ~jkff_SR() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
    QString vhdlCode(int);
    QString verilogCode(int);
};

#endif /* jkff_SR_H */
