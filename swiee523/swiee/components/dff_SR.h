#ifndef dff_SR_H
#define dff_SR_H

#include "component.h"

class dff_SR : public Component
{
  public:
    dff_SR();
    ~dff_SR() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
    QString vhdlCode(int);
    QString verilogCode(int);
};

#endif /* dff_SR_H */
