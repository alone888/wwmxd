#ifndef DLS_1ton_H
#define DLS_1ton_H

#include "component.h"

class DLS_1ton : public Component
{
  public:
    DLS_1ton();
    ~DLS_1ton() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
};

#endif /* DLS_1ton_H */
