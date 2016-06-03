#ifndef DLS_nto1_H
#define DLS_nto1_H

#include "component.h"

class DLS_nto1 : public Component
{
  public:
    DLS_nto1();
    ~DLS_nto1() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
};

#endif /* DLS_nto1_H */
