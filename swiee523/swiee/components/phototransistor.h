#ifndef phototransistor_H
#define phototransistor_H

#include "component.h"

class phototransistor : public Component
{
  public:
    phototransistor();
    ~phototransistor() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
};

#endif /* phototransistor_H */
